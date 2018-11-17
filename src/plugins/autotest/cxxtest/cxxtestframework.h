#ifndef CXXTESTFRAMEWORK_H
#define CXXTESTFRAMEWORK_H

#include <autotest/itestframework.h>

namespace Autotest {
	namespace Internal {

		class CxxTestFramework : public ITestFramework
		{
		public:
			CxxTestFramework() : ITestFramework(true) {}
			const char *name() const override;
			unsigned priority() const override;
			bool hasFrameworkSettings() const override;
			IFrameworkSettings *createFrameworkSettings() const override;
			ITestSettingsPage *createSettingsPage(QSharedPointer<IFrameworkSettings> settings) const override;
			QString groupingToolTip() const override;

		protected:
			ITestParser *createTestParser() const override;
			TestTreeItem *createRootNode() const override;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTFRAMEWORK_H
