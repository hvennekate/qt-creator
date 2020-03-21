#ifndef CXXTESTPARSER_H
#define CXXTESTPARSER_H

#include <autotest/itestparser.h>

namespace Autotest {
	namespace Internal {

		class CxxTestTreeItem;

		class CxxTestParseResult : public TestParseResult
		{
		public:
			explicit CxxTestParseResult(ITestFramework *fw);
			TestTreeItem *createTestTreeItem() const override;
			bool matches(const TestTreeItem *item) const;
		};

		class CxxTestParser : public CppParser
		{
		private:
			QSet<QString> parentTestClasses(const QString &fileName, QSet<QString> visitedFileNames) const;
			CxxTestParseResult *generateClassParseResult(CPlusPlus::Symbol *symbol, QString proFile, const QString &fileName);
			CxxTestParseResult *prepareTestParseResult(const QString &fileName, const QString &proFile, CPlusPlus::Symbol *symbol, TestTreeItem::Type type);

		public:
			explicit CxxTestParser(ITestFramework *framework) : CppParser(framework) {}
			bool processDocument(QFutureInterface<TestParseResultPtr> futureInterface, const QString &fileName) override;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTPARSER_H
