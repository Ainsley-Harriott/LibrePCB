#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget* parent, const librepcb::Version& appVersion, const QString& gitVersion, const QString& buildDate) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    // Set up base dialog
    ui->setupUi(this);

    // Event handlers
    connect(mUi->buttonBox, &QDialogButtonBox::clicked,
            this, &AboutDialog::close);

    // Content alignment
    ui->horizontalLayout->setAlignment(ui->logo, Qt::AlignTop);
    ui->aboutContentLayout->setAlignment(Qt::AlignTop);

    // Add content to about dialog
    QLabel *textIntro = new QLabel(tr("LibrePCB is a free & open source schematic/layout-editor."));
    QLabel *headerVersion = new QLabel("<b>" + tr("Version Info") + "</b>");
    QLabel *textVersion = new QLabel(QString("%1 (%2), built on %3.").arg(appVersion.toPrettyStr(3), gitVersion, buildDate));
    QLabel *headerLinks = new QLabel("<b>" + tr("Links") + "</b>");
    QLabel *textLinks = new QLabel(tr("For more information, please check out <a href='%1'>librepcb.org</a><br>or our <a href='%2'>GitHub repository</a>.").arg("http://librepcb.org/", "https://github.com/LibrePCB/LibrePCB"));
    QLabel *headerLicense = new QLabel("<b>" + tr("License") + "</b>");
    QLabel *textLicense = new QLabel(tr("LibrePCB is free software, released under the GNU General<br>Public License (GPL) version 3 or later. You can find the full<br>license text <a href='https://github.com/LibrePCB/LibrePCB/blob/master/LICENSE.txt'>in our source code</a>."));
    headerVersion->setContentsMargins(0, 16, 0, 0);
    headerLinks->setContentsMargins(0, 16, 0, 0);
    headerLicense->setContentsMargins(0, 16, 0, 0);
    textLinks->setOpenExternalLinks(true);
    textLicense->setOpenExternalLinks(true);
    ui->aboutContentLayout->addWidget(textIntro);
    ui->aboutContentLayout->addWidget(headerVersion);
    ui->aboutContentLayout->addWidget(textVersion);
    ui->aboutContentLayout->addWidget(headerLinks);
    ui->aboutContentLayout->addWidget(textLinks);
    ui->aboutContentLayout->addWidget(headerLicense);
    ui->aboutContentLayout->addWidget(textLicense);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
