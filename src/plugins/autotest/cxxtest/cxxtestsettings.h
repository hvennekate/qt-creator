/****************************************************************************
**
** Copyright (C) 2020 Hendrik Vennekate
** Contact: Hendrik.Vennekate@posteo.de
**
** This file extends Qt Creator to support a customized version of CxxTest.
** (see https://sourceforge.net/projects/cxxtest/)
**
** GNU General Public License Usage
** This file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#ifndef CXXTESTSETTINGS_H
#define CXXTESTSETTINGS_H

#include <autotest/iframeworksettings.h>

namespace Autotest {
	namespace Internal {

		class CxxTestSettings : public IFrameworkSettings
		{
		public:
			CxxTestSettings() = default;

			QString name() const override;

		protected:
			void toFrameworkSettings(QSettings *s) const override;
			void fromFrameworkSettings(const QSettings *s) override;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTSETTINGS_H
