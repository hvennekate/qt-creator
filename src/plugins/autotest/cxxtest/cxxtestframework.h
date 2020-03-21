#ifndef CXXTESTFRAMEWORK_H
#define CXXTESTFRAMEWORK_H

#include <autotest/itestframework.h>

namespace Autotest {
	namespace Internal {

		class CxxTestSettings;
		class CxxTestSettingsPage;

		class CxxTestFramework : public ITestFramework
		{
		public:
			CxxTestFramework();

		private:
			const char *name() const override;
			unsigned priority() const override;
			IFrameworkSettings *frameworkSettings() override;
			QString groupingToolTip() const override;
			ITestParser *createTestParser() override;
			TestTreeItem *createRootNode() const override;

			QScopedPointer<CxxTestSettings> m_settings;
			QScopedPointer<CxxTestSettingsPage> m_settingsPage;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTFRAMEWORK_H
