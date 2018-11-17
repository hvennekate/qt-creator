#ifndef CXXTESTPARSER_H
#define CXXTESTPARSER_H

#include <autotest/itestparser.h>

namespace Autotest {
	namespace Internal {

		class CxxTestParser : public CppParser
		{
		public:

			// ITestParser interface
		public:
			bool processDocument(QFutureInterface<TestParseResultPtr> futureInterface, const QString &fileName) override;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTPARSER_H
