/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2018 LibrePCB Developers, see AUTHORS.md for contributors.
 * http://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <librepcb/common/application.h>

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {

AboutDialog::AboutDialog(QWidget* parent) noexcept :
    QDialog(parent),
    mUi(new Ui::AboutDialog)
{
    // Set up base dialog
    mUi->setupUi(this);

    // Event handlers
    connect(mUi->buttonBox, &QDialogButtonBox::clicked,
            this, &AboutDialog::close);

    // Content alignment
    mUi->horizontalLayout->setAlignment(mUi->logo, Qt::AlignTop);
    mUi->aboutContentLayout->setAlignment(Qt::AlignTop);

    // Get some version information
    const Version& appVersion = qApp->getAppVersion();
    const QString& gitVersion = qApp->getGitVersion();
    const QString& buildDate = qApp->getBuildDate().toString("yyyy-MM-dd hh:mm:ss (t)");

    // Dynamic text
    mUi->headerVersion->setText("<b>" + tr("Version Info") + "</b>");
    mUi->textVersion->setText(QString("%1 (%2)<br>Build date: %3").arg(appVersion.toPrettyStr(3), gitVersion, buildDate));
    mUi->headerLinks->setText("<b>" + tr("Links") + "</b>");
    mUi->textLinks->setText(tr("For more information, please check out <a href='%1'>librepcb.org</a> or our <a href='%2'>GitHub repository</a>.").arg("http://librepcb.org/", "https://github.com/LibrePCB/LibrePCB"));
    mUi->headerLicense->setText("<b>" + tr("License") + "</b>");

    // Format content
    formatLabelHeading(mUi->headerVersion);
    formatLabelHeading(mUi->headerLinks);
    formatLabelHeading(mUi->headerLicense);
    formatLabelText(mUi->textIntro, false, false);
    formatLabelText(mUi->textVersion, true, false);
    formatLabelText(mUi->textLinks, false, true);
    formatLabelText(mUi->textLicense, false, true);
}

/**
 * @brief Format a heading label in the about dialog.
 * @param label Pointer to the QLabel instance
 */
void AboutDialog::formatLabelHeading(QLabel* label) noexcept
{
    int headerMarginTop = 8;
    label->setContentsMargins(0, headerMarginTop, 0, 0);
}

/**
 * @brief Format a text label in the about dialog.
 * @param label Pointer to the QLabel instance
 * @param selectable Whether to make the text mouse-selectable
 * @param containsLinks Whether to open links in external application (e.g. web browser)
 */
void AboutDialog::formatLabelText(QLabel* label, bool selectable, bool containsLinks) noexcept
{
    label->setOpenExternalLinks(containsLinks);
    if (selectable) {
        label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        if (containsLinks) {
            qWarning() << "If text is selectable, external links won't work anymore!";
        }
    }
}

AboutDialog::~AboutDialog() noexcept
{
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb
