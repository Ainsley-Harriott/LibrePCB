#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget* parent, const librepcb::Version& appVersion, const QString& gitVersion) :
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
    QLabel *headerVersion = new QLabel("<b>" + tr("Version") + "</b>");
    QLabel *textVersion = new QLabel(QString("%1 (%2)").arg(appVersion.toPrettyStr(3), gitVersion));
    QLabel *headerLinks = new QLabel("<b>" + tr("Link") + "</b>");
    QLabel *textLink = new QLabel(tr("Please see <a href='http://librepcb.org/'>librepcb.org</a> for more information."));
    QLabel *textGithub = new QLabel(tr("You can find the project on GitHub:<br><a href='%1'>%1</a>").arg("https://github.com/LibrePCB/LibrePCB"));
    headerVersion->setContentsMargins(0, 16, 0, 0);
    headerLinks->setContentsMargins(0, 16, 0, 0);
    textLink->setOpenExternalLinks(true);
    textGithub->setOpenExternalLinks(true);
    ui->aboutContentLayout->addWidget(textIntro);
    ui->aboutContentLayout->addWidget(headerVersion);
    ui->aboutContentLayout->addWidget(textVersion);
    ui->aboutContentLayout->addWidget(headerLinks);
    ui->aboutContentLayout->addWidget(textLink);
    ui->aboutContentLayout->addWidget(textGithub);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
