#include "cxxtestsettingspage.h"
#include "autotest/autotestconstants.h"
#include "cxxtestconstants.h"
#include "QCoreApplication"

namespace Autotest {
	namespace Internal {

		class CxxTestSettingsPageWidget : public Core::IOptionsPageWidget
		{
		public:
			CxxTestSettingsPageWidget(CxxTestSettings* settings) { Q_UNUSED(settings) }
			void apply() override;
		};

		void CxxTestSettingsPageWidget::apply()
		{
			// TODO apply settings
		}

		CxxTestSettingsPage::CxxTestSettingsPage(CxxTestSettings *settings, Core::Id settingsId)
		{
			setId(settingsId);
			setCategory(Constants::AUTOTEST_SETTINGS_CATEGORY);
			setDisplayName(QCoreApplication::translate(CxxTest::Constants::FRAMEWORK_NAME,
			                                           CxxTest::Constants::FRAMEWORK_SETTINGS_CATEGORY));
			setWidgetCreator([settings] { return new CxxTestSettingsPageWidget(settings); });
		}


	} // namespace Internal
} // namespace Autotest
