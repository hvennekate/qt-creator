#ifndef CXXTESTPARSER_H
#define CXXTESTPARSER_H

#include <autotest/itestparser.h>

namespace Autotest {
	namespace Internal {

		class CxxTestTreeItem;

		class CxxTestParseResult : public TestParseResult
		{
		public:
			explicit CxxTestParseResult(const Core::Id &id);
			TestTreeItem *createTestTreeItem() const override;
			bool matches(const TestTreeItem *item) const;
		};

		class CxxTestParser : public CppParser
		{
		private:
			QSet<QString> parentTestClasses(const QString &fileName, QSet<QString> visitedFileNames) const;
			CxxTestParseResult *generateClassParseResult(CPlusPlus::Symbol *symbol, QString proFile, const QString &fileName);
		public:
			bool processDocument(QFutureInterface<TestParseResultPtr> futureInterface, const QString &fileName) override;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTPARSER_H
