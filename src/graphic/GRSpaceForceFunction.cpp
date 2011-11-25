/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifdef OLDSPFACTIVE

#include "GRSpaceForceFunction.h"
#include "GRSpringForceIndex.h"

// The code for the GRSpaceForceFunction

GRSpaceForceFunction::GRSpaceForceFunction()
: cl(1)
{
	matmemsize = 50;
	matrealsize = 0;

	mat = (double *) malloc(matmemsize * matmemsize * sizeof(double));

	memset(mat,0,matmemsize*matmemsize*sizeof(double));

	mscmatmemsize = 50;
	mscmatrealsize = 0;

	mscmat = (double *) malloc(mscmatmemsize * 4 * 
		sizeof(double));

	memset(mscmat,0,mscmatmemsize*4*sizeof(double));

	// now, we add just the initial value ...
	hash = new KR_HashTable<float,GRHashEntry>();

	GRHashEntry he;
	he.extent = 0;


	hash->Set(0.0,he);

	freezeoffset = 0;

	// the zero-constant is important
	// for the case, where, there is just
	// a hash-entry for zero (and also springs
	// but with zero-extended springs).
	// in this case, the extent/force must be
	// calculated with the saved spring-constants.
	// The starting value must be done this
	// way to ensure, that adding spring-constants
	// always works in the same way.
	zconstnum = 1.0;
	zconstdenom = 0.0;

	evalret = 0.0;
	evalextent = -10.0;

	forceret = -1.0;
	forceextent = -10.0;
}

GRSpaceForceFunction::~GRSpaceForceFunction()
{
	if (mat)
		free(mat);
	if (mscmat)
		free(mscmat);
	delete hash;
}


// addSpring adds a Spring to the SPF. The saved
// force is inserted in the hash (that is, we
// look, whether a force exists). 
// If not, a new Hash-Entry is created. The hash
// is sorted (according to the force-keys). All
// entries with a smaller force get there extents
// increased by the initial extent of spr. The ones
// right of the given force get a force-dependant
// increase of their extent.
// The extent at the given force is calculated by
// taking the inital extent (force = 0) and 
// adding all extents of springs at the given force
// and subtracting the initial extents.
// If an entry is already there, then only the extents
// left (and at the current position) are increased
// statically; the greater forces are adjusted
// dependant on the spring/force.

// We assume, that the hash is sorted (force) and
// we assert, that it will be sorted when
// the procedure exits.
void GRSpaceForceFunction::addSpring(GRSpring *spr)
{

	// this is the newxtent, if there is no
	// hash-entry yet ...
	float newextent = 0.0;

	// first ,we adjust all entries with
	// a smaller or equal force.
	// as the hash is sorted, we 
	// just need to 
	// iterate as long as we are smaller.
	GuidoPos pos = hash->GetHeadPosition();
	while (pos)
	{
		KR_HashTable<float,GRHashEntry>::KeyType *kt = NULL;
		if (hash->GetNext(pos,&kt))
		{
			assert(kt);
			
			float frc = kt->key;
			GRHashEntry *he = &kt->val;

			if (frc <= spr->force)
			{
				// static increment
				he->extent += spr->x;

				if (frc < spr->force)
				{
					GuidoPos sprpos = he->sprlst->GetHeadPosition();
					while (sprpos)
					{
						GRSpring *tmpspr = he->sprlst->GetNext(sprpos);
						if (!tmpspr->isfrozen)
						{
							newextent += spr->force/tmpspr->sconst;
							newextent -= tmpspr->x;
						}
						else
							newextent += tmpspr->x;
					}
				}
			}
			else
			{
				// increment according to
				// spring and force ...
				he->extent += frc/spr->sconst;
			}
			
		}
		
	}
	
	
	// look, whether there is an entry already:
	// GRSpaceForceFunction::
	GRHashEntry *hashentry = NULL;
	hash->Lookup(spr->force,&hashentry);
	if (hashentry)
	{
		// there is an entry already there
		
		// then, we just add the spring to
		// the SpringList ...
		hashentry->sprlst->AddTail(spr);
	}
	else
	{
		// no entry yet ...
		// Then we create an entry ...
		GRHashEntry he;
		
		// The newextent must add the ZERO-Value
		
		hash->Lookup(0.0,&hashentry);
		if (hashentry)
		{
			newextent += hashentry->extent;
		}
		else
			assert(false);
		
		
		he.extent = newextent;


		he.sprlst->AddHead(spr);
		
		assert(spr->force>=0);
		hash->Set(spr->force,he);
		
		hash->sortKeys();
		
	}

	// this adds the spring (without adding it)
	// and puts it at a location of force 100 ...
	if (spr->force == 0.0)
	{
		// then, we have to add the constant
		// to the zero-constant (zconst)
		// n_i+1 = n_i * c_i
		// d_i+1 = d_i * c_i + n_i
		// Summed constant = n_i/d_i
		zconstdenom = zconstnum + zconstdenom * spr->sconst;
		zconstnum = zconstnum * spr->sconst;
	}

	evalextent = -10.0;
	forceextent = -10.0;
}

// This procedure removes a spring from
// the hash-> The extents are adjusted
// accordingly, that is, left or equal to
// the entry, the springs initial extent
// is subtracted; greater force get the
// usual force-dependant extent-subtraction.
void GRSpaceForceFunction::deleteSpring(GRSpring *spr)
{

	GRHashEntry *hashentry  = NULL;
	hash->Lookup(spr->force,&hashentry);
	if (hashentry)
	{
		if (hashentry->sprlst->GetElementPos(spr) != NULL)
		{
			
			GuidoPos pos = hash->GetHeadPosition();
			while (pos)
			{
				KR_HashTable<float,GRHashEntry>::KeyType *kt = NULL;
				if (hash->GetNext(pos,&kt))
				{
					assert(kt);
					
					float frc = kt->key;
					GRHashEntry *he = &kt->val;
					
					if (frc<= spr->force)
					{
						// static decrement
						he->extent -= spr->x;					
					}
					else
					{
						// decrement according to
						// spring and force ...
						
						he->extent -= frc/spr->sconst;
					}
					
				} // if hash->GetNext()
			} // while (pos)
			
			hashentry->sprlst->RemoveElement(spr);
			
			// ATTENTION, the ZERO-entry must
			// be maintained ... Otherwise
			// the initial lookup would be wrong.
			if (hashentry->sprlst->GetCount() == 0
				&& spr->force != 0.0)
			{
				// also remove the hashentry ...
				hash->Delete(spr->force);
				
			}
		}
	} // if hashentry

	if (spr->force == 0.0)
	{
		// then, we have to remove the spring
		// from our calculated constant
		// according to the formula:
		// n_i+1 = n_i / c
		// d_i+1 = (d_i - n_i+1) / c
		zconstnum = zconstnum / spr->sconst;
		zconstdenom = (zconstdenom - zconstnum) / spr->sconst;

	}

	evalextent = -10.0;
	forceextent = -10.0;

	// delete spring with the matrix-functions
	// is just like setting columns and rows to zero ...
}

/** \brief Changes the Force for a given Spring and adjusts the 
	extents accordingly.

	Right now, this is realised by just calling deleteSpring and then addSpring
*/
void GRSpaceForceFunction::changeSpringForce(GRSpring *spr,float newforce)
{
	assert(false);
	deleteSpring(spr);
	spr->force = newforce;
	addSpring(spr);
}

/** \brief Gets the extent when a given force is exerted.

	First, the interval is determined, then the
	extent is calulated by linear interpolition.
*/
float GRSpaceForceFunction::getExtent(float force) const
{
	// the hash is sorted by forces ...

	GRHashEntry *he = NULL;
	hash->Lookup(0.0,&he);
	assert(he);

	// the initial extent ...
	float lastextent = he->extent;

	if (force == 0.0)
		return he->extent;
	
	if (force<0)
		assert(false);

	// special-case: we have have just one entry
	// (the zero-entry)
	if (hash->GetCount() == 1)
	{
		// now, we check the zeroconst ...
		if (zconstdenom != 0.0)
		{
			// then, we have a valid zero-springconstant
			float sconst = zconstnum / zconstdenom;
			assert(sconst>0.0);
			return force/sconst;
		}

		// then, we have an invalid zeroconst.
		// Then it will be always the zeroextent
		// regardless of the given force ....
		return lastextent;

	}

	float lastforce = 0.0;

	GuidoPos pos = hash->GetHeadPosition();
	while (pos)
	{
		KR_HashTable<float,GRHashEntry>::KeyType *kt = NULL;
		if (hash->GetNext(pos,&kt))
		{
			assert(kt);

			if (kt->key < force)
			{
				lastextent = kt->val.extent;
				lastforce = kt->key;
				continue;
			}

			assert(kt->key>0.0);

			// a simple linear interpolation.
			return 
				(( kt->val.extent - lastextent ) /
				 ( kt->key  - lastforce) *
				 (force - lastforce) ) + 
				 lastextent;

		} // if (hash->GetNext())



	}

	// if we are here, then the force is greater
	// then any saved force ....

	// then we are linear from the last entry.

	float retval = force / lastforce * (lastextent-freezeoffset) + freezeoffset;
	assert(retval>=lastextent);
	return retval;
	
}

/** \brief This calculate the force with a given extent.
	This is analogous to the getExtent-Function.

	We look through the hash and find the
	matching segment, then we interpolate linearly.
	if the extent is smaller than the initial extent,
	-1 is returned.
*/
float GRSpaceForceFunction::getForce(float extent ofstream * springlog)
{
	if (springlog)
	{
		int j;
		double *matrix = this->getMatrix();
		int realsize = this->getMatrixRealSize();
		int memsize = this->getMatrixMemSize();
		int i;
				
		*springlog << realsize-1 << ", " << cl.GetCount() 
			<< ", 1" << endl << endl;
		
		*springlog  << "*fx = ";
		for (i=0;i<realsize;i++)
		{
			for (j=i;j<realsize;j++)
			{
				double value = *(matrix + i + memsize *j);
				if (value > 0)
				{
					*springlog << "+ ";
					if (i!=j)
						value *= 2;
					*springlog << 
						value << "* x[" << i << "] * x["
						<< j << "] "; 
				}
			}
			*springlog << endl;			
		}
		
		*springlog << ";" <<  endl;
		
		// now the derives
		*springlog << endl;
		for (i=0;i<realsize;i++)
		{
			int isprinted = 0;
			for (j=0;j<realsize;j++)
			{
				double value = *(matrix + i + memsize *j);
				if (value > 0)
				{
					if (!isprinted)
					{
						*springlog << "gfx[" << i << "] = ";
						isprinted = 1;
					}
					
					*springlog << "+ " << 
						2*value << "* x["
						<< j << "] "; 
				}
				// springlog << *(matrix+i+memsize*j) << " ";
			}
			if (isprinted)
				*springlog << ";" << endl;
		}
		
		// equality constraint:
		
		// now we look at the matrix stuff...
		*springlog << endl << "hx[1] = ";
		for (i=1;i<realsize;i++)
			*springlog << "+ x[" << i << "] ";
		*springlog << "- " << extent << " ; " << endl << endl; 
		
		// ghx ....
		for (i=1;i<realsize;i++)
		{
			*springlog << "ghx[" << 300+i << "] = 1;" << endl;
		}
		
		*springlog << endl;
		int counter=1;
		// constraints ...
		GuidoPos pos = cl.GetHeadPosition();
		while (pos)
		{
			GRConstraint *ct = cl.GetNext(pos);
			
			*springlog << "gx[" << counter++ << "] = ";
			
			int i;
			for (i=ct->start;i<ct->end;i++)
				*springlog << "+ x[" << i << "] ";
			*springlog << "- " << ct->value << ";" << endl;
			
		}
		
		// derivation der constraints ...
		// ggx[301 ...] 
		// ggx[309 ...]
		
		// ggx[601 ...]
		
		*springlog << endl;
		counter = 1;
		pos = cl.GetHeadPosition();
		while (pos)
		{
			GRConstraint *ct = cl.GetNext(pos);
			
			
			for (j=1;j<realsize;j++)
			{
				*springlog << "ggx[" << 300*counter + j << "] = ";
				if (ct->start <= j && j < ct->end)
					*springlog << "1;" << endl;
				else
					*springlog << "0;" << endl;
				
			}
			counter ++;
			// springlog << endl;
		}
		

	forceextent = extent;

	GRHashEntry *he = NULL;
	hash->Lookup(0.0,&he);
	assert(he);

	if (he->extent > extent)
	{
		forceret = -1;
		return -1;
	}

	if (extent <= he->extent)
	{
		forceret = 0.0;
		return 0.0; // no force needed!
	}

	float lastextent = he->extent;
	float lastforce = 0.0;

	// special-case: we have have just one entry
	// (the zero-entry)
	if (hash->GetCount() == 1)
	{
		// now, we check the zeroconst ...
		if (zconstdenom != 0.0)
		{
			// then, we have a valid zero-springconstant
			float sconst = zconstnum / zconstdenom;
			assert(sconst>0.0);
			forceret = extent * sconst;
			return forceret;
		}

		// then, we have an invalid zeroconst.
		// Then it will be always the zeroforce
		// regardless of the given extent ....
		forceret = -1.0;
		return -1.0;

	}

	GuidoPos pos = hash->GetHeadPosition();
	while (pos)
	{
		KR_HashTable<float,GRHashEntry>::KeyType * kt = NULL;
		if (hash->GetNext(pos,&kt))
		{
			assert(kt);

			if (kt->val.extent < extent)
			{
				lastextent = kt->val.extent;
				lastforce = kt->key;
				continue;

			}

			assert(kt->val.extent > 0.0);

			if (kt->val.extent - lastextent == 0.0)
			{
				forceret = lastforce;
				return forceret;
			}

			// a simple linear interpolation.
			// what about FREEZE-OFFSET?!?!?!?
			// think about this!!!!
			float slope = ( kt->key  - lastforce) /
				 ( kt->val.extent - lastextent );
			forceret = 
				 slope * (extent - lastextent)  + 
				 lastforce;
			return forceret;

		} // if (hash->GetNext())



	}

	// if we are here, then the extent is greater
	// then any saved extent ...

	// then we are linear from the last entry.


	long l = floor(lastextent * 1000.0+0.5);
	assert(l>=0);
	lastextent = l / 1000.0;
	l = floor(freezeoffset * 1000.0+0.5);
	assert(l>=0);
	freezeoffset = l / 1000.0;
	if (lastextent == freezeoffset)
	{
		// then we will not be able
		// to reach the extent, because
		// all springs are frozen
		return -1;
	}

	forceret = (extent-freezeoffset) / 
		(lastextent-freezeoffset) * lastforce;
	assert(forceret>=lastforce);
	return forceret;
	
}

/** \brief Adds another space-force-function

	This is done to alculate the actual SPF for a StaffManager
	(multiple SPFs are maintained, one for each breakpoint)
*/
void GRSpaceForceFunction::addSPF( const GRSpaceForceFunction & spf)
{
	// first, we take a look and
	// see, if there are any entries yet.

	// first, we save the current hash-table
	// and create an empty one....
	KR_HashTable<float,GRHashEntry> *newhash =
		new KR_HashTable<float,GRHashEntry>;


	// this is a NONOPTIMIZED version ...
	// we travers the two functions sequentially
	// (not in parallel!)

	GuidoPos pos = hash->GetHeadPosition();
	while (pos)
	{
		KR_HashTable<float,GRHashEntry>::KeyType *kt = NULL;
		if (hash->GetNext(pos,&kt))
		{
			assert(kt);
			// now we have the entry.

			float extent = kt->val.extent;
			extent += spf.getExtent(kt->key);

			GRHashEntry he;
			he.extent = extent;
			assert(kt->key>=0);
			newhash->Set(kt->key,he);

			// now we copy the springlists.
			GRHashEntry *hashentry;
			newhash->Lookup(kt->key,&hashentry);
			assert(hashentry);

			// add the Springlist of the 
			// original....
			hashentry->sprlst->DumpListAtTail( kt->val.sprlst );
			delete kt->val.sprlst;
			kt->val.sprlst = 0;

			// now check, wether there is a
			// matching entry in the second SPF
			GRHashEntry * he2;
			if (spf.hash->Lookup(kt->key,&he2))
			{
				SpringList tempList ( *he2->sprlst );  // new SpringList(*he2->sprlst));
				hashentry->sprlst->DumpListAtTail( &tempList );
			}
		}
	}

	// now we travers the second 


	pos = spf.hash->GetHeadPosition();

	while (pos)
	{
		KR_HashTable<float,GRHashEntry>::KeyType * kt = 0;

		if (spf.hash->GetNext(pos,&kt))
		{
			GRHashEntry *hashentry;
			if (newhash->Lookup(kt->key,&hashentry))
			{
				// already there.
				// then, this position was
				// handled in the first while-loop
			}
			else
			{
				// new entry ...

				float extent = kt->val.extent;
				// this increments the
				// extent by the ORGINAL
				// function!
				extent += this->getExtent(kt->key);

				GRHashEntry he;
				he.extent = extent;

				assert(kt->key>=0.0);
				newhash->Set(kt->key,he);

				// now, we get the REAL Address of
				// the HashEntry.
				GRHashEntry *hashentry;
				newhash->Lookup(kt->key,&hashentry);
				assert(hashentry);

				SpringList tempList ( *kt->val.sprlst ); // new SpringList(*kt->val.sprlst));
				hashentry->sprlst->DumpListAtTail( &tempList );
			}
		}
	}

	// now we reset the hash ...

	// first, save the old hash (so it can be
	// properly deleted).
	KR_HashTable<float,GRHashEntry> * tmphash = hash;

	// now we set the hash to the newly calculated one
	hash = newhash;

	hash->sortKeys();

	// the freezeoffsets are added ...
	freezeoffset += spf.freezeoffset;

	// now, the zeroconstants are added ...
	if (spf.zconstdenom > 0.0)
	{
		float spfconst = spf.zconstnum /
			spf.zconstdenom;
		zconstdenom = zconstdenom * spfconst + 
			zconstnum;
		zconstnum = zconstnum * spfconst;
	}
	
	// the previous hash is delete.
	delete tmphash;

	evalextent = -10.0;
	forceextent = -10.0;



	// now we deal with the matrix-stuff

	if (spf.mscmatrealsize > mscmatrealsize)
		resizeMSCMatrix(spf.mscmatrealsize);

	// now we copy/add the elements ....
	int i;
	int j;
	for (i=0;i<mscmatrealsize;i++)
		for (j=0;j<4;j++)
		{
			*(mscmat + i + j*mscmatmemsize) +=
				*(spf.mscmat + i + j*spf.mscmatmemsize);
		}


	if (spf.matrealsize > matrealsize)
		resizeMatrix(spf.matrealsize);

	// now we copy/add the elements ....
	for (i=0;i<matrealsize;i++)
		for (j=0;j<matrealsize;j++)
		{
			*(mat + i + j*matmemsize) +=
				*(spf.mat + i + j*spf.matmemsize);
		}

	// now we have to add the constraints ....
	pos = spf.cl.GetHeadPosition();
	while (pos)
	{
		cl.AddTail(new GRConstraint(*spf.cl.GetNext(pos)));
	}

}

/** \brief This routine copies a given spf into
	the current spf; the springs are ignored.
*/
void GRSpaceForceFunction::CopySPFWithoutSprings(const GRSpaceForceFunction &spf)
{
	// first, we need to remove any hash
	// that is present ... (most important the
	// zero-entry)

	delete hash;
	hash = new KR_HashTable<float,GRHashEntry>();

	GuidoPos pos = spf.hash->GetHeadPosition();
	while (pos)
	{

		KR_HashTable<float,GRHashEntry>::KeyType * kt = NULL;

		if (spf.hash->GetNext(pos,&kt))
		{
			GRHashEntry he;
			he.extent = kt->val.extent;

			assert(kt->key>=0.0);
			hash->Set(kt->key,he);
		}
	}

	freezeoffset = spf.freezeoffset;

	zconstnum = spf.zconstnum;
	zconstdenom = spf.zconstdenom;

	evalextent = -10.0;
	forceextent = -10.0;
}

/** \brief Resets the force of all springs within its range.
	so that it matches the key.
*/
void GRSpaceForceFunction::ResetSprings()
{
	GuidoPos pos = hash->GetHeadPosition();
	while (pos)
	{
		KR_HashTable<float,GRHashEntry>::KeyType * kt = NULL;
		if (hash->GetNext(pos,&kt))
		{
			assert(kt);
			// now we have the entry.

			// now, we travers the springlist.
			GuidoPos pos = kt->val.sprlst->GetHeadPosition();
			while (pos)
			{
				GRSpring *spr = kt->val.sprlst->GetNext(pos);
				spr->change_force(kt->key);
			}

		}
	}

}

/** \brief Freezes a spring in the space-force-function.

	it reduces the extent of the entries with
	a greater force, so that they don't 
	expect the spring to be stretched.
	The procedure also increases the freeze-
	offset to correctly calculate the needed
	force/extent.
*/
void GRSpaceForceFunction::FreezeSpring(GRSpring * spr)
{
	assert(spr);

	// first ,we adjust all entries with
	// a greater force.
	// as the hash is sorted, we 
	// just need to 
	// iterate directly. 
	GuidoPos pos = hash->GetHeadPosition();
	while (pos)
	{
		KR_HashTable<float,GRHashEntry>::KeyType * kt = NULL;
		if (hash->GetNext(pos,&kt))
		{
			assert(kt);
			
			float frc = kt->key;
			GRHashEntry *he = &kt->val;

			if (frc> spr->force)
			{
				// this removes the variable
				// part.
				he->extent -= frc/spr->sconst;

				// this adds the static part.
				he->extent += spr->x;
			}
			
		}
		
	}
	
	freezeoffset += spr->x;	

	// if the spring is a zero-force-spring, then
	// the zero-constant must be adjusted.
	// analogous to deletespring.
	if (spr->force == 0.0)
	{
		// then, we have to remove the spring
		// from our calculated constant
		// according to the formula:
		// n_i+1 = n_i / c
		// d_i+1 = (d_i - n_i+1) / c
		zconstnum = zconstnum / spr->sconst;
		zconstdenom = (zconstdenom - zconstnum) / spr->sconst;

	}

	evalextent = -10.0;
	forceextent = -10.0;


}

/** \brief Unfreeze a spring in the spaceforcefunction.

	The freezeoffset is decreased, and
	those entries with a bigger force are expanded anteilig.
*/
void GRSpaceForceFunction::UnfreezeSpring(GRSpring * spr)
{
	assert(spr);

	GuidoPos pos = hash->GetHeadPosition();
	while (pos)
	{
		KR_HashTable<float,GRHashEntry>::KeyType * kt = NULL;
		if (hash->GetNext(pos,&kt))
		{
			assert(kt);
			
			float frc = kt->key;
			GRHashEntry *he = &kt->val;

			if (frc> spr->force)
			{

				// this adds the variable
				// part.
				he->extent += frc/spr->sconst;


				// this removes the static part.
				he->extent -= spr->x;

			}
			
		}
		
	}

	freezeoffset -= spr->x;

	// analogous to addspring, we must
	// change the zeroconst ...
	if (spr->force == 0.0)
	{
		// then, we have to add the constant
		// to the zero-constant (zconst)
		// n_i+1 = n_i * c_i
		// d_i+1 = d_i * c_i + n_i
		// Summed constant = n_i/d_i
		zconstdenom = zconstnum + zconstdenom * spr->sconst;
		zconstnum = zconstnum * spr->sconst;
	}

	evalextent = -10.0;
	forceextent = -10.0;
}

// this routine must be analyzed and
// adjusted to its need. Right now it
// is just a linear function 
// (adjust the parameters ...)
float GRSpaceForceFunction::getOptimumForce(float sprconst)
{
	assert(false);
	// returns the optimum force for a given
	// springconst ....

	assert(sprconst>=0);

	return (sprconst * (- 400.0/30.0 ) + 400.0 );
}

/**	\brief This routine evaluates a break-position
	It gets the wanted extent and then determines an evaluation-value.
	
	A value of 1 is most desirable ...
	everything bigger or smaller is worse.
	it works as follows: first, the
	extent with zero force (ex0) is determined
	(just the first hash-entry).
	Then, the force for reaching the wanted extent
	is calculated. Then, for each spring in the hash, 
	the distance to the optimum force is calculated
	and evaluated on a scale from 0.0 to 1.0
	All those values are added and afterwards divided
	through the number of springs. 
	This value is then returned.
*/
float GRSpaceForceFunction::EvaluateBreak(float extent)
{
	if (extent == evalextent)
		return evalret;
	float ex0 = getExtent(0.0);
	float neededforce = getForce(extent);
	float optimum = optimumforce;

	evalextent = extent;
	evalret = 0.0;

	if (neededforce <= 0)
	{
		evalret = 0;
		return evalret;
	}
	if (neededforce <= optimum)
	{
		evalret = optimum / neededforce;
		return evalret;
	}
	else
	{
		if (neededforce >= 2* optimum)
		{
			evalret = 0.0;
			return evalret;
		}
		evalret = 1.0 - (neededforce-optimum) / optimum;
		return evalret;
	}

	return evalret;
}

/** \brief This is another constructor for copying the complete spf (including the springs)
*/
GRSpaceForceFunction::GRSpaceForceFunction(const GRSpaceForceFunction &spf)
	:cl(1)
{
	
	// first, we need to remove any hash
	// that is present ... (most important the
	// zero-entry)

	hash = new KR_HashTable<float,GRHashEntry>();

	GuidoPos pos = spf.hash->GetHeadPosition();
	while (pos)
	{

		KR_HashTable<float,GRHashEntry>::KeyType * kt = 0;

		if (spf.hash->GetNext(pos,&kt))
		{
			GRHashEntry he;
			he.extent = kt->val.extent;

			assert(kt->key>=0.0);
			hash->Set(kt->key,he);

			// now, we need to copy the springlist
			GRHashEntry *hashentry;
			hash->Lookup(kt->key,&hashentry);
			assert(hashentry);
			
			SpringList tempList ( *kt->val.sprlst ); // new SpringList(*kt->val.sprlst));
			hashentry->sprlst->DumpListAtTail( &tempList );
		}
	}

	freezeoffset = spf.freezeoffset;

	zconstnum = spf.zconstnum;
	zconstdenom = spf.zconstdenom;

	evalextent = -10.0;
	forceextent = -10.0;

	mscmatmemsize = spf.mscmatmemsize;
	mscmat = (double *) malloc(mscmatmemsize * 4 * 
		sizeof(double));
	mscmatrealsize =spf.mscmatrealsize;

	memcpy(mscmat,spf.mscmat,mscmatmemsize* 4
		*sizeof(double));

	matmemsize = spf.matmemsize;
	mat = (double *) malloc(matmemsize * matmemsize * sizeof(double));
	matrealsize =spf.matrealsize;

	memcpy(mat,spf.mat,matmemsize*matmemsize*sizeof(double));

	// copy constraints ...
	pos = spf.cl.GetHeadPosition();
	while (pos)
	{
		cl.AddTail(new GRConstraint(*spf.cl.GetNext(pos)));
	}
}

/** \brief Resizes the matrix if necessary
*/
double * GRSpaceForceFunction::resizeMSCMatrix(int newrealsize)
{
	if (newrealsize > mscmatmemsize)
	{
		int newmemsize = newrealsize + 10;
		double *newmat = (double *) malloc(newmemsize *
			4 * sizeof(double));

		memset(newmat,0,sizeof(double)*newmemsize*4);
		// copy old matrix entries
		// this is wrong ....
		int i;
		int j;
		for (i=0;i<mscmatmemsize;i++)
		{
			for (j=0;j<4;j++)
			{
				*(newmat + i + j*newmemsize) = *(mscmat + i +
					j*mscmatmemsize);
			}
		}

		mscmatmemsize = newmemsize;

		double *oldmat = mscmat;
		mscmat = newmat;

		free(oldmat);
	}

	mscmatrealsize = newrealsize;

	return mscmat;
}

double GRSpaceForceFunction::getMSCMatrix(int i,int j) const
{
	if (i>=0 && i< mscmatrealsize &&
		j>=0 && j< 4)
	{
		return *(mscmat + i + j*mscmatmemsize); 
	}
	return 0;
}

void GRSpaceForceFunction::setMSCMatrix(int i,int j,double value)
{
	if (i>=0 && i< mscmatrealsize &&
		j>=0 && j< 4)
	{
		*(mscmat + i + j*mscmatmemsize) = value;
	}

}

double * GRSpaceForceFunction::getMSCMatrix()
{
	return mscmat;

}

int GRSpaceForceFunction::getMSCMatrixMemSize()
{

	return mscmatmemsize;
}

int GRSpaceForceFunction::getMSCMatrixRealSize()
{
	return mscmatrealsize;

}

/** \brief Resizes the matrix if necessary.
*/
double * GRSpaceForceFunction::resizeMatrix(int newrealsize)
{
	if (newrealsize > matmemsize)
	{
		int newmemsize = newrealsize + 10;
		double * newmat = (double *) malloc(newmemsize *
			newmemsize * sizeof(double));

		memset(newmat,0,sizeof(double)*newmemsize*newmemsize);
		// copy old matrix entries
		// this is wrong ....
		int i;
		int j;
		for (i=0;i<matmemsize;i++)
		{
			for (j=0;j<matmemsize;j++)
			{
				*(newmat + i + j*newmemsize) = *(mat + i +
					j*matmemsize);
			}
		}

		matmemsize = newmemsize;

		double * oldmat = mat;
		mat = newmat;

		if (oldmat)
			free(oldmat);
	}

	matrealsize = newrealsize;

	return mat;
}

double * GRSpaceForceFunction::getMatrix()
{
	return mat;
}

int GRSpaceForceFunction::getMatrixMemSize()
{
	return matmemsize;
}

int GRSpaceForceFunction::getMatrixRealSize()
{
	return matrealsize;
}

/** \brief Adds a constraint for the QP-Problem.
*/
int GRSpaceForceFunction::addConstraint(int start, int end, double value)
{

	GRConstraint * ct = new GRConstraint(start,end,value);

	bool found = false;
	GuidoPos pos = cl.GetHeadPosition();
	while (pos)
	{
		if (*cl.GetNext(pos) == *ct)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		cl.AddTail(ct);
		return 1;
	}
	else
		delete ct;

	return 0;
}

GRSpring * GRSpaceForceFunction::getGRSpring(int id)
{
	GuidoPos pos = hash->GetHeadPosition();
	while (pos)
	{
		KR_HashTable<float,GRHashEntry>::KeyType * kt = NULL;
		if (hash->GetNext(pos,&kt))
		{
			assert(kt);
			
			float frc = kt->key;
			GRHashEntry * he = &kt->val;
			if (he->sprlst)
			{
				GuidoPos sprpos = he->sprlst->GetHeadPosition();
				while (sprpos)
				{
					GRSpring *spr = he->sprlst->GetNext(sprpos);
					if (spr->getID() == id)
						return spr;
				}
			}
		}
	}
	return NULL;

}

#endif // OLDSPFACTIVE
