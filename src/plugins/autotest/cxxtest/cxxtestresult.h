#ifndef CXXTESTRESULT_H
#define CXXTESTRESULT_H

#include "../testresult.h"

namespace Autotest {
	namespace Internal {

		class CxxTestResult : public TestResult
		{
		public:
			CxxTestResult(const QString &id, const QString &name, const TestResult *parent = nullptr);
			CxxTestResult(const QString &id, const QString &method, const QString &testClass,  const TestResult *parent = nullptr);
			const QString outputString(bool selected) const override;
			const TestTreeItem *findTestTreeItem() const override;
			bool isDirectParentOf(const TestResult *other, bool *needsIntermediate) const override;
		private:
			QString testClass;
			const TestResult *parent;

		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTRESULT_H
