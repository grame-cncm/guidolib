#ifndef GUIDOSTREAMSESSION_H
#define GUIDOSTREAMSESSION_H

#include "guidosession.h"
#include "GUIDOParse.h"
#include <string>

namespace guidohttpd
{

/*!
 * \brief The guidostreamsession class. Specialized guidosession to use the GuidoStream API.
 */
class guidostreamsession : public guidosession
{
	public:
		/*!
		 * \brief guidostreamsession. Create a new Session. Initialise the guidoStream.
		 * \param svgfontfile svg font file
		 * \param id id of the session
		 */
		guidostreamsession(const std::string &svgfontfile, std::string id);
		/*!
		 * \brief ~guidostreamsession. Delete guidosession and delete guido stream.
		 */
		virtual ~guidostreamsession();
		/*!
		 * \brief addData add data to the stream. Create the ar and gr.
		 * \param gmnCode code to add.
		 * \return
		 */
		guidoAPIresponse addData(std::string gmnCode);
		/*!
		 * \brief reset Reset the stream.
		 * \return
		 */
		guidoAPIresponse reset();

		/*!
		 * \brief sFileBegin String to identify cache file created from guidostreamsession.
		 */
		static const std::string sFileBegin;
	private:
		/*!
		 * \brief fStream The guido stream.
		 */
		GuidoStream *fStream;
};

}
#endif // GUIDOSTREAMSESSION_H
