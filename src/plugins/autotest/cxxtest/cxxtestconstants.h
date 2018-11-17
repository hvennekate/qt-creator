#ifndef CXXTESTCONSTANTS_H
#define CXXTESTCONSTANTS_H

#pragma once

#include <QtGlobal>

namespace Autotest {
	namespace CxxTest {
		namespace Constants {
			const char FRAMEWORK_NAME[] = "CxxTest";
			const char FRAMEWORK_SETTINGS_CATEGORY[] = QT_TRANSLATE_NOOP("CxxTestFramework", "Cxx Tests");
			const unsigned FRAMEWORK_PRIORITY = 20;
			const char DEFAULT_FILTER[] = "*.h";
		}
	}
}

#endif // CXXTESTCONSTANTS_H
