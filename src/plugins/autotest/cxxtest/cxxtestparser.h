#ifndef CXXTESTPARSER_H
#define CXXTESTPARSER_H

#include <autotest/itestparser.h>

namespace Autotest {
	namespace Internal {

		class CxxTestParseResult : public TestParseResult
		{
		public:
			explicit CxxTestParseResult(const Core::Id &id);
			TestTreeItem *createTestTreeItem() const override;
		};

		class CxxTestParser : public CppParser
		{
		public:
			bool processDocument(QFutureInterface<TestParseResultPtr> futureInterface, const QString &fileName) override;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTPARSER_H
