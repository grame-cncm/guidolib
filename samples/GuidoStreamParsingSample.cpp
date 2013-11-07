#include "GUIDOParse.h"

int main()
{
    ARHandler ar;

	/* Stream and parser creation */
    GuidoStream *newStream = GuidoOpenStream();
	GuidoParser *newParser = GuidoOpenParser();

	/* Stream writing */
    GuidoWriteStream(newStream, "[a b");

	/* AR recuperation */
    ar = GuidoStream2AR(newParser, newStream);

	/* etc. */
    GuidoWriteStream(newStream, "{a");

    ar = GuidoStream2AR(newParser, newStream);

    GuidoWriteStream(newStream, ", b}");

    ar = GuidoStream2AR(newParser, newStream);

    GuidoWriteStream(newStream, "\\cluster<color=\"blue\"");

    ar = GuidoStream2AR(newParser, newStream);

    GuidoWriteStream(newStream, ", dx=5");

    ar = GuidoStream2AR(newParser, newStream);

    GuidoWriteStream(newStream, ">({a, b2}) a");

    ar = GuidoStream2AR(newParser, newStream);

    GuidoWriteStream(newStream, "8");

    ar = GuidoStream2AR(newParser, newStream);

    /* Stream flushing */
    GuidoFlushStream(newStream);

    GuidoWriteStream(newStream, "[ a");

    ar = GuidoStream2AR(newParser, newStream);

    GuidoWriteStream(newStream, "b ]");

    ar = GuidoStream2AR(newParser, newStream);

	/* Stream and parser closure */
    GuidoCloseStream(newStream);
	GuidoCloseParser(newParser);

	return 0;
}