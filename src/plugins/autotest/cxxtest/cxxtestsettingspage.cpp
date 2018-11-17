#include "cxxtestsettingspage.h"

namespace Autotest {
	namespace Internal {

		struct CxxTestSettingsPagePrivate {
			QPointer<QWidget> widget;
		};

		CxxTestSettingsPage::CxxTestSettingsPage(QSharedPointer<IFrameworkSettings> settings, const ITestFramework *framework)
			: ITestSettingsPage(framework),
			  d_ptr(new CxxTestSettingsPagePrivate)
		{

		}

		QWidget *CxxTestSettingsPage::widget()
		{
			Q_D(CxxTestSettingsPage);
			if (!d->widget) d->widget = new QWidget;
			return d->widget;
		}

		void CxxTestSettingsPage::apply()
		{
			Q_D(CxxTestSettingsPage);
			if (!d->widget) return;

			// TODO apply settings
		}
	} // namespace Internal
} // namespace Autotest
