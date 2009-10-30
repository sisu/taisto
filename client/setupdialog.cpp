#include"setupdialog.h"
#include<QFormLayout>
#include<QDialogButtonBox>
SetupDialog::SetupDialog(QWidget* parent):QDialog(parent)
{
	QFormLayout* lay = new QFormLayout();

	setLayout(lay);
	nameEdit = new QLineEdit();
	nameEdit->setText("player");
	lay->addRow("Name",nameEdit);
	hostEdit = new QLineEdit();
	lay->addRow("Host address",hostEdit);
	soundCheck = new QCheckBox();
	soundCheck->setChecked(true);
	lay->addRow("Enable sound",soundCheck);
	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
			                                      | QDialogButtonBox::Cancel);
	lay->addWidget(buttonBox);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
QString SetupDialog::name()const
{
	return nameEdit->text();
}
QString SetupDialog::hostAddress()const
{
	return hostEdit->text();
}

bool SetupDialog::sound()const
{
	return soundCheck->isChecked();
}


