#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)//constructer
    : QMainWindow(parent)//calls the base class
    , ui(new Ui::MainWindow)//intializes and creates a new design
{
    ui->setupUi(this);

    // Initialize the network manager
    manager = new QNetworkAccessManager(this);//handles network operations-sending,receiving

    // Prepare and populate the country list
    prepare_country_list();//calls function and their currencies in combo box
    populate_country_list();

    // Connect the button click signal to the slot
    connect(ui->btnGetRate, &QPushButton::clicked, this, &MainWindow::on_BTN_get_rate_clicked);
}//slot response to a function
//destructer cleans up after the object is destroyed

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BTN_get_rate_clicked()
{
    // Extract the currency codes from the selected items in the combo boxes
    QString sourceCurrency = ui->comboBoxSourceCountry->currentText().split(" ").last();
    QString destinationCurrency = ui->comboBoxDestinationCountry->currentText().split(" ").last();

    // Fetch the exchange rate
    fetch_exchange_rate(sourceCurrency, destinationCurrency);
}

void MainWindow::fetch_exchange_rate(const QString &sourceCurrency, const QString &destinationCurrency)
{
    // Construct the API URL
    QString url = QString("https://v6.exchangerate-api.com/v6/3c3ac8efffd47dcfb970eb4f/latest/USD").arg(sourceCurrency);

    // Create a network request
    QNetworkRequest request;
    request.setUrl(QUrl(url));

    // Send the GET request
    QNetworkReply *reply = manager->get(request);

    // Connect the reply's finished signal to the slot for handling the response
    connect(reply, &QNetworkReply::finished, this, [this, reply, destinationCurrency]() {
        if (reply) {
            on_data_received(reply, destinationCurrency);
        }
    });
}

void MainWindow::on_data_received(QNetworkReply *reply, const QString &destinationCurrency)
{
    // Check for network errors
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::warning(this, "Error", "Network error: " + reply->errorString());
        reply->deleteLater();
        return;
    }

    // Read the response data
    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    //Debug: print the raw json response
    qDebug() << "API Response:" << responseData;


    // Parse the JSON response
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    if (jsonDoc.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to parse JSON response.");
        return;
    }

    // Extract the exchange rate for the destination currency
    QJsonObject jsonObj = jsonDoc.object();
    QJsonObject rates = jsonObj["conversion_rates"].toObject();

    //Debud: print all available currency codesin the response
    qDebug() << "Available Currencies in Response:" << rates.keys();


    // Check if the destination currency exists in the rates
    if (!rates.contains(destinationCurrency)) {
        QMessageBox::warning(this, "Error", "Currency not found in the response.");
        return;
    }

    double exchangeRate = rates[destinationCurrency].toDouble();

    // Update the UI with the exchange rate
    ui->lblSourceCountry->setText(QString("1 %1 = %2 %3")
                                      .arg(ui->comboBoxSourceCountry->currentText().split(" ").last())
                                      .arg(exchangeRate)
                                      .arg(destinationCurrency));
}

void MainWindow::prepare_country_list()
{
    // Populate the country list with currency information
    countryList << "AFGHANISTAN Afghani AFN"
                << "ALBANIA Lek ALL"
                << "ALGERIA Algerian Dinar DZD"
                << "AMERICAN SAMOA US Dollar USD"
                << "ANDORRA Euro EUR"
                << "ANGOLA Kwanza AOA"
                << "ANGUILLA East Caribbean Dollar XCD"
                << "ANTARCTICA No universal currency"
                << "ANTIGUA AND BARBUDA East Caribbean Dollar XCD"
                << "ARGENTINA Argentine Peso ARS"
                << "ARMENIA Armenian Dram AMD"
                << "ARUBA Aruban Florin AWG"
                << "AUSTRALIA Australian Dollar AUD"
                << "AUSTRIA Euro EUR"
                << "AZERBAIJAN Azerbaijanian Manat AZN"
                << "BAHAMAS Bahamian Dollar BSD"
                << "BAHRAIN Bahraini Dinar BHD"
                << "BANGLADESH Taka BDT"
                << "BARBADOS Barbados Dollar BBD"
                << "BELARUS Belarussian Ruble BYN"
                << "BELGIUM Euro EUR"
                << "BELIZE Belize Dollar BZD"
                << "BENIN CFA Franc XOF"
                << "BERMUDA Bermudian Dollar BMD"
                << "BHUTAN Ngultrum BTN"
                << "BOLIVIA Boliviano BOB"
                << "BOSNIA AND HERZEGOVINA Convertible Mark BAM"
                << "BOTSWANA Pula BWP"
                << "BRAZIL Brazilian Real BRL"
                << "BRUNEI DARUSSALAM Brunei Dollar BND"
                << "BULGARIA Bulgarian Lev BGN"
                << "BURKINA FASO CFA Franc XOF"
                << "BURUNDI Burundi Franc BIF"
                << "CABO VERDE Cabo Verde Escudo CVE"
                << "CAMBODIA Riel KHR"
                << "CAMEROON CFA Franc XAF"
                << "CANADA Canadian Dollar CAD"
                << "CAYMAN ISLANDS Cayman Islands Dollar KYD"
                << "CENTRAL AFRICAN REPUBLIC CFA Franc XAF"
                << "CHAD CFA Franc XAF"
                << "CHILE Chilean Peso CLP"
                << "CHINA Yuan Renminbi CNY"
                << "COLOMBIA Colombian Peso COP"
                << "COMOROS Comoro Franc KMF"
                << "CONGO Congolese Franc CDF"
                << "COSTA RICA Costa Rican Colon CRC"
                << "CROATIA Euro EUR"
                << "CUBA Cuban Peso CUP"
                << "CYPRUS Euro EUR"
                << "CZECH REPUBLIC Czech Koruna CZK"
                << "DENMARK Danish Krone DKK"
                << "DJIBOUTI Djibouti Franc DJF"
                << "DOMINICA East Caribbean Dollar XCD"
                << "DOMINICAN REPUBLIC Dominican Peso DOP"
                << "ECUADOR US Dollar USD"
                << "EGYPT Egyptian Pound EGP"
                << "EL SALVADOR US Dollar USD"
                << "EQUATORIAL GUINEA CFA Franc XAF"
                << "ERITREA Nakfa ERN"
                << "ESTONIA Euro EUR"
                << "ETHIOPIA Ethiopian Birr ETB"
                << "EUROPEAN UNION Euro EUR"
                << "FIJI Fiji Dollar FJD"
                << "FINLAND Euro EUR"
                << "FRANCE Euro EUR"
                << "GABON CFA Franc XAF"
                << "GAMBIA Dalasi GMD"
                << "GEORGIA Lari GEL"
                << "GERMANY Euro EUR"
                << "GHANA Ghana Cedi GHS"
                << "GIBRALTAR Gibraltar Pound GIP"
                << "GREECE Euro EUR"
                << "GRENADA East Caribbean Dollar XCD"
                << "GUATEMALA Quetzal GTQ"
                << "GUINEA Guinean Franc GNF"
                << "GUINEA-BISSAU CFA Franc XOF"
                << "GUYANA Guyana Dollar GYD"
                << " HAITI Gourde HTG"
                << "HONDURAS Lempira HNL"
                << "HONG KONG Hong Kong Dollar HKD"
                << "HUNGARY Forint HUF"
                << "ICELAND Iceland Krona ISK"
                << "INDIA Indian Rupee INR"
                << "INDONESIA Rupiah IDR"
                << "IRAN Iranian Rial IRR"
                << "IRAQ Iraqi Dinar IQD"
                << "IRELAND Euro EUR"
                << "ISRAEL New Israeli Sheqel ILS"
                << "ITALY Euro EUR"
                << "JAMAICA Jamaican Dollar JMD"
                << "JAPAN Yen JPY"
                << "JORDAN Jordanian Dinar JOD"
                << "KAZAKHSTAN Tenge KZT"
                << "KENYA Kenyan Shilling KES"
                << "KIRIBATI Australian Dollar AUD"
                << "KUWAIT Kuwaiti Dinar KWD"
                << "KYRGYZSTAN Som KGS"
                << "LAOS Kip LAK"
                << "LATVIA Euro EUR"
                << "LEBANON Lebanese Pound LBP"
                << "LESOTHO Loti LSL"
                << "LIBERIA Liberian Dollar LRD"
                << "LIBYA Libyan Dinar LYD"
                << "LIECHTENSTEIN Swiss Franc CHF"
                << "LITHUANIA Euro EUR"
                << "LUXEMBOURG Euro EUR"
                << "MACAO Pataca MOP"
                << "MADAGASCAR Malagasy Ariary MGA"
                << "MALAWI Kwacha MWK"
                << "MALAYSIA Malaysian Ringgit MYR"
                << "MALDIVES Rufiyaa MVR"
                << "MALI CFA Franc XOF"
                << "MALTA Euro EUR"
                << "MARSHALL ISLANDS US Dollar USD"
                << "MAURITANIA Ouguiya MRU"
                << "MAURITIUS Mauritius Rupee MUR"
                << "MEXICO Mexican Peso MXN"
                << "MICRONESIA US Dollar USD"
                << "MOLDOVA Moldovan Leu MDL"
                << "MONACO Euro EUR"
                << "MONGOLIA Tugrik MNT"
                << "MONTENEGRO Euro EUR"
                << "MOROCCO Moroccan Dirham MAD"
                << "MOZAMBIQUE Mozambique Metical MZN"
                << "MYANMAR Kyat MMK"
                << "NAMIBIA Namibia Dollar NAD"
                << "NAURU Australian Dollar AUD"
                << "NEPAL Nepalese Rupee NPR"
                << "NETHERLANDS Euro EUR"
                << "NEW ZEALAND New Zealand Dollar NZD"
                << "NICARAGUA Cordoba Oro NIO"
                << "NIGER CFA Franc XOF"
                << "NIGERIA Naira NGN"
                << "NORWAY Norwegian Krone NOK"
                << "OMAN Rial Omani OMR"
                << "PAKISTAN Pakistan Rupee PKR"
                << "PALAU US Dollar USD"
                << "PANAMA Balboa PAB"
                << "PAPUA NEW GUINEA Kina PGK"
                << "PARAGUAY Guarani PYG"
                << "PERU Nuevo Sol PEN"
                << "PHILIPPINES Philippine Peso PHP"
                << "POLAND Zloty PLN"
                << "PORTUGAL Euro EUR"
                << "QATAR Qatari Rial QAR"
                << "ROMANIA Leu RON"
                << "RUSSIA Russian Ruble RUB"
                << "RWANDA Rwandan Franc RWF"
                << "SAINT KITTS AND NEVIS East Caribbean Dollar XCD"
                << "SAINT LUCIA East Caribbean Dollar XCD"
                << "SAINT VINCENT AND THE GRENADINES East Caribbean Dollar XCD"
                << "SAMOA Tala WST"
                << "SAN MARINO Euro EUR"
                << "SAO TOME AND PRINCIPE Dobra STN"
                << "SAUDI ARABIA Saudi Riyal SAR"
                << "SENEGAL CFA Franc XOF"
                << "SERBIA Serbian Dinar RSD"
                << "SEYCHELLES Seychellois Rupee SCR"
                << "SIERRA LEONE Leone SLL"
                << "SINGAPORE Singapore Dollar SGD"
                << "SLOVAKIA Euro EUR"
                << "SLOVENIA Euro EUR"
                << "SOLOMON ISLANDS Solomon Islands Dollar SBD"
                << "SOMALIA Somali Shilling SOS"
                << "SOUTH AFRICA Rand ZAR"
                << "SOUTH SUDAN South Sudanese Pound SSP"
                << "SPAIN Euro EUR"
                << "SRI LANKA Sri Lanka Rupee LKR"
                << "SUDAN Sudanese Pound SDG"
                << "SURINAME Surinamese Dollar SRD"
                << "SWEDEN Swedish Krona SEK"
                << "SWITZERLAND Swiss Franc CHF"
                << "SYRIA Syrian Pound SYP"
                << "TAIWAN New Taiwan Dollar TWD"
                << "TAJIKISTAN Somoni TJS"
                << "TANZANIA Tanzanian Shilling TZS"
                << "THAILAND Baht THB"
                << "TIMOR-LESTE US Dollar USD"
                << "TOGO CFA Franc XOF"
                << "TONGA Pa'anga TOP"
                << "TRINIDAD AND TOBAGO Trinidad and Tobago Dollar TTD"
                << "TUNISIA Tunisian Dinar TND"
                << "TURKEY Turkish Lira TRY"
                << "TURKMENISTAN Manat TMT"
                << "TUVALU Australian Dollar AUD"
                << "UGANDA Ugandan Shilling UGX"
                << "UKRAINE Hryvnia UAH"
                << "UNITED ARAB EMIRATES UAE Dirham AED"
                << "UNITED KINGDOM Pound Sterling GBP"
                << "UNITED STATES US Dollar USD"
                << "URUGUAY Uruguayan Peso UYU"
                << "UZBEKISTAN Uzbekistani Som UZS"
                << "VANUATU Vatu VUV"
                << "VATICAN CITY Euro EUR"
                << "VENEZUELA Bolivar VEF"
                << "VIETNAM Dong VND"
                << "YEMEN Yemeni Rial YER"
                << "ZAMBIA Zambian Kwacha ZMW"
                << "ZIMBABWE Zimbabwe Dollar ZWL";
}

void MainWindow::populate_country_list()
{
    // Add the country list to the combo boxes
    ui->comboBoxSourceCountry->addItems(countryList);
    ui->comboBoxDestinationCountry->addItems(countryList);
}
