/* i18n.cpp — Translated CLI strings for uchardet.
 *
 * Each language provides a UchardetStrings struct. The locale is detected
 * from LC_ALL > LC_MESSAGES > LANG environment variables, matching the
 * first two characters (language code) against the table below.
 *
 * Error messages include the English original for searchability:
 * "(<localized 'search for'>: <English error text>)" so non-English
 * speakers can still find solutions online.
 *
 * 30 languages matching the README translations:
 * en, ar, az, bn, de, el, es, fa, fr, he, hi, hu, it, ja,
 * km, ko, pa, pl, ps, pt, ro, ru, sw, ta, th, tr, uk, ur, vi, zh
 */

#include "i18n.h"
#include <cstdlib>
#include <cstring>

/* ── English (default fallback) ─────────────────────────────────── */
static const UchardetStrings strings_en = {
    "uchardet Command Line Tool",
    "Version %s",
    "Authors: %s",
    "Bug Report: %s",
    "Usage:",
    " uchardet [Options] [File]...",
    "Options:",
    " -v, --version         Print version and build information.",
    " -h, --help            Print this help.",
    "unknown",
    "Handle data error.",
    "Please use %s --help.",
};

/* ── Arabic ──────────────────────────────────────────────────────── */
static const UchardetStrings strings_ar = {
    "أداة سطر أوامر uchardet",
    "الإصدار %s",
    "المؤلفون: %s",
    "الإبلاغ عن خطأ: %s",
    "الاستخدام:",
    " uchardet [خيارات] [ملف]...",
    "الخيارات:",
    " -v, --version         طباعة معلومات الإصدار.",
    " -h, --help            طباعة هذه المساعدة.",
    "غير معروف",
    "خطأ في معالجة البيانات. (ابحث عن: Handle data error)",
    "استخدم %s --help.",
};

/* ── Azerbaijani ─────────────────────────────────────────────────── */
static const UchardetStrings strings_az = {
    "uchardet əmr sətiri aləti",
    "Versiya %s",
    "Müəlliflər: %s",
    "Xəta hesabatı: %s",
    "İstifadə:",
    " uchardet [Seçimlər] [Fayl]...",
    "Seçimlər:",
    " -v, --version         Versiya məlumatını göstər.",
    " -h, --help            Bu yardımı göstər.",
    "naməlum",
    "Verilənlərin emalı xətası. (axtarın: Handle data error)",
    "Zəhmət olmasa %s --help istifadə edin.",
};

/* ── Bengali ─────────────────────────────────────────────────────── */
static const UchardetStrings strings_bn = {
    "uchardet কমান্ড লাইন টুল",
    "সংস্করণ %s",
    "লেখক: %s",
    "বাগ রিপোর্ট: %s",
    "ব্যবহার:",
    " uchardet [বিকল্প] [ফাইল]...",
    "বিকল্প:",
    " -v, --version         সংস্করণ তথ্য দেখান।",
    " -h, --help            এই সাহায্য দেখান।",
    "অজানা",
    "ডেটা প্রক্রিয়াকরণে ত্রুটি। (অনুসন্ধান করুন: Handle data error)",
    "অনুগ্রহ করে %s --help ব্যবহার করুন।",
};

/* ── German ──────────────────────────────────────────────────────── */
static const UchardetStrings strings_de = {
    "uchardet Kommandozeilenwerkzeug",
    "Version %s",
    "Autoren: %s",
    "Fehlerbericht: %s",
    "Verwendung:",
    " uchardet [Optionen] [Datei]...",
    "Optionen:",
    " -v, --version         Versions- und Build-Informationen anzeigen.",
    " -h, --help            Diese Hilfe anzeigen.",
    "unbekannt",
    "Fehler bei der Datenverarbeitung. (suchen nach: Handle data error)",
    "Bitte verwenden Sie %s --help.",
};

/* ── Greek ───────────────────────────────────────────────────────── */
static const UchardetStrings strings_el = {
    "Εργαλείο γραμμής εντολών uchardet",
    "Έκδοση %s",
    "Συγγραφείς: %s",
    "Αναφορά σφάλματος: %s",
    "Χρήση:",
    " uchardet [Επιλογές] [Αρχείο]...",
    "Επιλογές:",
    " -v, --version         Εμφάνιση πληροφοριών έκδοσης.",
    " -h, --help            Εμφάνιση αυτής της βοήθειας.",
    "άγνωστο",
    "Σφάλμα επεξεργασίας δεδομένων. (αναζήτηση: Handle data error)",
    "Παρακαλώ χρησιμοποιήστε %s --help.",
};

/* ── Spanish ─────────────────────────────────────────────────────── */
static const UchardetStrings strings_es = {
    "Herramienta de línea de comandos uchardet",
    "Versión %s",
    "Autores: %s",
    "Reporte de errores: %s",
    "Uso:",
    " uchardet [Opciones] [Archivo]...",
    "Opciones:",
    " -v, --version         Mostrar información de versión.",
    " -h, --help            Mostrar esta ayuda.",
    "desconocido",
    "Error al procesar datos. (buscar: Handle data error)",
    "Por favor use %s --help.",
};

/* ── Persian ─────────────────────────────────────────────────────── */
static const UchardetStrings strings_fa = {
    "ابزار خط فرمان uchardet",
    "نسخه %s",
    "نویسندگان: %s",
    "گزارش اشکال: %s",
    "استفاده:",
    " uchardet [گزینه‌ها] [فایل]...",
    "گزینه‌ها:",
    " -v, --version         نمایش اطلاعات نسخه.",
    " -h, --help            نمایش این راهنما.",
    "ناشناخته",
    "خطا در پردازش داده‌ها. (جستجو کنید: Handle data error)",
    "لطفاً از %s --help استفاده کنید.",
};

/* ── French ──────────────────────────────────────────────────────── */
static const UchardetStrings strings_fr = {
    "Outil en ligne de commande uchardet",
    "Version %s",
    "Auteurs : %s",
    "Signaler un bogue : %s",
    "Utilisation :",
    " uchardet [Options] [Fichier]...",
    "Options :",
    " -v, --version         Afficher les informations de version.",
    " -h, --help            Afficher cette aide.",
    "inconnu",
    "Erreur lors du traitement des données. (rechercher: Handle data error)",
    "Veuillez utiliser %s --help.",
};

/* ── Hebrew ──────────────────────────────────────────────────────── */
static const UchardetStrings strings_he = {
    "כלי שורת פקודה uchardet",
    "גרסה %s",
    "מחברים: %s",
    "דיווח על באג: %s",
    "שימוש:",
    " uchardet [אפשרויות] [קובץ]...",
    "אפשרויות:",
    " -v, --version         הצג פרטי גרסה.",
    " -h, --help            הצג עזרה זו.",
    "לא ידוע",
    "שגיאה בעיבוד נתונים. (חפש: Handle data error)",
    "אנא השתמש ב %s --help.",
};

/* ── Hindi ───────────────────────────────────────────────────────── */
static const UchardetStrings strings_hi = {
    "uchardet कमांड लाइन टूल",
    "संस्करण %s",
    "लेखक: %s",
    "बग रिपोर्ट: %s",
    "उपयोग:",
    " uchardet [विकल्प] [फ़ाइल]...",
    "विकल्प:",
    " -v, --version         संस्करण जानकारी दिखाएं।",
    " -h, --help            यह सहायता दिखाएं।",
    "अज्ञात",
    "डेटा प्रोसेसिंग में त्रुटि। (खोजें: Handle data error)",
    "कृपया %s --help का उपयोग करें।",
};

/* ── Hungarian ───────────────────────────────────────────────────── */
static const UchardetStrings strings_hu = {
    "uchardet parancssori eszköz",
    "Verzió %s",
    "Szerzők: %s",
    "Hibajelentés: %s",
    "Használat:",
    " uchardet [Opciók] [Fájl]...",
    "Opciók:",
    " -v, --version         Verzióinformáció megjelenítése.",
    " -h, --help            Ezen súgó megjelenítése.",
    "ismeretlen",
    "Adatfeldolgozási hiba. (keresés: Handle data error)",
    "Kérjük használja a %s --help parancsot.",
};

/* ── Italian ─────────────────────────────────────────────────────── */
static const UchardetStrings strings_it = {
    "Strumento a riga di comando uchardet",
    "Versione %s",
    "Autori: %s",
    "Segnalazione bug: %s",
    "Utilizzo:",
    " uchardet [Opzioni] [File]...",
    "Opzioni:",
    " -v, --version         Mostra le informazioni sulla versione.",
    " -h, --help            Mostra questo aiuto.",
    "sconosciuto",
    "Errore nell'elaborazione dei dati. (cercare: Handle data error)",
    "Si prega di usare %s --help.",
};

/* ── Japanese ────────────────────────────────────────────────────── */
static const UchardetStrings strings_ja = {
    "uchardet コマンドラインツール",
    "バージョン %s",
    "作者: %s",
    "バグ報告: %s",
    "使用法:",
    " uchardet [オプション] [ファイル]...",
    "オプション:",
    " -v, --version         バージョン情報を表示。",
    " -h, --help            このヘルプを表示。",
    "不明",
    "データ処理エラー。 (検索: Handle data error)",
    "%s --help をご利用ください。",
};

/* ── Khmer ───────────────────────────────────────────────────────── */
static const UchardetStrings strings_km = {
    "ឧបករណ៍បន្ទាត់ពាក្យបញ្ជា uchardet",
    "កំណែ %s",
    "អ្នកនិពន្ធ: %s",
    "រាយការណ៍បញ្ហា: %s",
    "ការប្រើប្រាស់:",
    " uchardet [ជម្រើស] [ឯកសារ]...",
    "ជម្រើស:",
    " -v, --version         បង្ហាញព័ត៌មានកំណែ។",
    " -h, --help            បង្ហាញជំនួយនេះ។",
    "មិនស្គាល់",
    "កំហុសក្នុងការដំណើរការទិន្នន័យ។ (ស្វែងរក: Handle data error)",
    "សូមប្រើ %s --help។",
};

/* ── Korean ──────────────────────────────────────────────────────── */
static const UchardetStrings strings_ko = {
    "uchardet 명령줄 도구",
    "버전 %s",
    "제작자: %s",
    "버그 보고: %s",
    "사용법:",
    " uchardet [옵션] [파일]...",
    "옵션:",
    " -v, --version         버전 정보를 표시합니다.",
    " -h, --help            이 도움말을 표시합니다.",
    "알 수 없음",
    "데이터 처리 오류. (검색: Handle data error)",
    "%s --help을 사용해 주세요.",
};

/* ── Punjabi ─────────────────────────────────────────────────────── */
static const UchardetStrings strings_pa = {
    "uchardet ਕਮਾਂਡ ਲਾਈਨ ਟੂਲ",
    "ਸੰਸਕਰਣ %s",
    "ਲੇਖਕ: %s",
    "ਬੱਗ ਰਿਪੋਰਟ: %s",
    "ਵਰਤੋਂ:",
    " uchardet [ਵਿਕਲਪ] [ਫ਼ਾਈਲ]...",
    "ਵਿਕਲਪ:",
    " -v, --version         ਸੰਸਕਰਣ ਜਾਣਕਾਰੀ ਦਿਖਾਓ।",
    " -h, --help            ਇਹ ਮਦਦ ਦਿਖਾਓ।",
    "ਅਗਿਆਤ",
    "ਡੇਟਾ ਪ੍ਰੋਸੈਸਿੰਗ ਤਰੁੱਟੀ। (ਖੋਜੋ: Handle data error)",
    "ਕਿਰਪਾ ਕਰਕੇ %s --help ਵਰਤੋ।",
};

/* ── Polish ──────────────────────────────────────────────────────── */
static const UchardetStrings strings_pl = {
    "Narzędzie wiersza poleceń uchardet",
    "Wersja %s",
    "Autorzy: %s",
    "Zgłoszenie błędu: %s",
    "Użycie:",
    " uchardet [Opcje] [Plik]...",
    "Opcje:",
    " -v, --version         Wyświetl informacje o wersji.",
    " -h, --help            Wyświetl tę pomoc.",
    "nieznany",
    "Błąd przetwarzania danych. (szukaj: Handle data error)",
    "Proszę użyć %s --help.",
};

/* ── Pashto ──────────────────────────────────────────────────────── */
static const UchardetStrings strings_ps = {
    "د uchardet کمانډ لاین وسیله",
    "نسخه %s",
    "لیکوال: %s",
    "د خطا راپور: %s",
    "کارونه:",
    " uchardet [اختیارونه] [فایل]...",
    "اختیارونه:",
    " -v, --version         د نسخه معلومات وښایئ.",
    " -h, --help            دا مرسته وښایئ.",
    "نامعلومه",
    "د ډیټا پروسېس کولو کې خطا. (لټون وکړئ: Handle data error)",
    "مهرباني وکړئ %s --help وکاروئ.",
};

/* ── Brazilian Portuguese ────────────────────────────────────────── */
static const UchardetStrings strings_pt = {
    "Ferramenta de linha de comando uchardet",
    "Versão %s",
    "Autores: %s",
    "Relatório de bugs: %s",
    "Uso:",
    " uchardet [Opções] [Arquivo]...",
    "Opções:",
    " -v, --version         Exibir informações de versão.",
    " -h, --help            Exibir esta ajuda.",
    "desconhecido",
    "Erro ao processar dados. (pesquisar: Handle data error)",
    "Por favor use %s --help.",
};

/* ── Romanian ────────────────────────────────────────────────────── */
static const UchardetStrings strings_ro = {
    "Instrumentul de linie de comandă uchardet",
    "Versiune %s",
    "Autori: %s",
    "Raport de erori: %s",
    "Utilizare:",
    " uchardet [Opțiuni] [Fișier]...",
    "Opțiuni:",
    " -v, --version         Afișează informații despre versiune.",
    " -h, --help            Afișează acest ajutor.",
    "necunoscut",
    "Eroare la procesarea datelor. (căutați: Handle data error)",
    "Vă rugăm folosiți %s --help.",
};

/* ── Russian ─────────────────────────────────────────────────────── */
static const UchardetStrings strings_ru = {
    "Инструмент командной строки uchardet",
    "Версия %s",
    "Авторы: %s",
    "Отчёт об ошибке: %s",
    "Использование:",
    " uchardet [Опции] [Файл]...",
    "Опции:",
    " -v, --version         Показать информацию о версии.",
    " -h, --help            Показать эту справку.",
    "неизвестно",
    "Ошибка обработки данных. (искать: Handle data error)",
    "Пожалуйста, используйте %s --help.",
};

/* ── Swahili ─────────────────────────────────────────────────────── */
static const UchardetStrings strings_sw = {
    "Zana ya amri ya uchardet",
    "Toleo %s",
    "Waandishi: %s",
    "Ripoti hitilafu: %s",
    "Matumizi:",
    " uchardet [Chaguo] [Faili]...",
    "Chaguo:",
    " -v, --version         Onyesha maelezo ya toleo.",
    " -h, --help            Onyesha msaada huu.",
    "haijulikani",
    "Hitilafu ya uchakataji data. (tafuta: Handle data error)",
    "Tafadhali tumia %s --help.",
};

/* ── Tamil ───────────────────────────────────────────────────────── */
static const UchardetStrings strings_ta = {
    "uchardet கட்டளை வரி கருவி",
    "பதிப்பு %s",
    "ஆசிரியர்: %s",
    "பிழை அறிக்கை: %s",
    "பயன்பாடு:",
    " uchardet [விருப்பங்கள்] [கோப்பு]...",
    "விருப்பங்கள்:",
    " -v, --version         பதிப்பு தகவல்களைக் காட்டு.",
    " -h, --help            இந்த உதவியைக் காட்டு.",
    "தெரியவில்லை",
    "தரவு பகுப்பாய்வு பிழை. (தேடுக: Handle data error)",
    "தயவுசெய்து %s --help பயன்படுத்தவும்.",
};

/* ── Thai ────────────────────────────────────────────────────────── */
static const UchardetStrings strings_th = {
    "เครื่องมือบรรทัดคำสั่ง uchardet",
    "เวอร์ชัน %s",
    "ผู้เขียน: %s",
    "รายงานบั๊ก: %s",
    "การใช้งาน:",
    " uchardet [ตัวเลือก] [ไฟล์]...",
    "ตัวเลือก:",
    " -v, --version         แสดงข้อมูลเวอร์ชัน",
    " -h, --help            แสดงความช่วยเหลือนี้",
    "ไม่ทราบ",
    "ข้อผิดพลาดในการประมวลผลข้อมูล (ค้นหา: Handle data error)",
    "กรุณาใช้ %s --help",
};

/* ── Turkish ─────────────────────────────────────────────────────── */
static const UchardetStrings strings_tr = {
    "uchardet komut satırı aracı",
    "Sürüm %s",
    "Yazarlar: %s",
    "Hata raporu: %s",
    "Kullanım:",
    " uchardet [Seçenekler] [Dosya]...",
    "Seçenekler:",
    " -v, --version         Sürüm bilgisini göster.",
    " -h, --help            Bu yardımı göster.",
    "bilinmiyor",
    "Veri işleme hatası. (arayın: Handle data error)",
    "Lütfen %s --help kullanın.",
};

/* ── Ukrainian ───────────────────────────────────────────────────── */
static const UchardetStrings strings_uk = {
    "Інструмент командного рядка uchardet",
    "Версія %s",
    "Автори: %s",
    "Звіт про помилку: %s",
    "Використання:",
    " uchardet [Опції] [Файл]...",
    "Опції:",
    " -v, --version         Показати інформацію про версію.",
    " -h, --help            Показати цю довідку.",
    "невідомо",
    "Помилка обробки даних. (шукати: Handle data error)",
    "Будь ласка, використовуйте %s --help.",
};

/* ── Urdu ────────────────────────────────────────────────────────── */
static const UchardetStrings strings_ur = {
    "uchardet کمانڈ لائن ٹول",
    "ورژن %s",
    "مصنفین: %s",
    "بگ رپورٹ: %s",
    "استعمال:",
    " uchardet [اختیارات] [فائل]...",
    "اختیارات:",
    " -v, --version         ورژن کی معلومات دکھائیں۔",
    " -h, --help            یہ مدد دکھائیں۔",
    "نامعلوم",
    "ڈیٹا پروسیسنگ میں خطا۔ (تلاش کریں: Handle data error)",
    "براہ مہربانی %s --help استعمال کریں۔",
};

/* ── Vietnamese ──────────────────────────────────────────────────── */
static const UchardetStrings strings_vi = {
    "Công cụ dòng lệnh uchardet",
    "Phiên bản %s",
    "Tác giả: %s",
    "Báo lỗi: %s",
    "Cách dùng:",
    " uchardet [Tùy chọn] [Tệp]...",
    "Tùy chọn:",
    " -v, --version         Hiển thị thông tin phiên bản.",
    " -h, --help            Hiển thị trợ giúp này.",
    "không xác định",
    "Lỗi xử lý dữ liệu. (tìm kiếm: Handle data error)",
    "Vui lòng sử dụng %s --help.",
};

/* ── Simplified Chinese ──────────────────────────────────────────── */
static const UchardetStrings strings_zh = {
    "uchardet 命令行工具",
    "版本 %s",
    "作者: %s",
    "错误报告: %s",
    "用法:",
    " uchardet [选项] [文件]...",
    "选项:",
    " -v, --version         显示版本信息。",
    " -h, --help            显示此帮助。",
    "未知",
    "数据处理错误。 (搜索: Handle data error)",
    "请使用 %s --help。",
};

/* ── Locale lookup table ─────────────────────────────────────────── */

struct LocaleEntry {
    const char* prefix;           /* 2-letter language code */
    const UchardetStrings* strings;
};

static const LocaleEntry locale_table[] = {
    { "ar", &strings_ar },
    { "az", &strings_az },
    { "bn", &strings_bn },
    { "de", &strings_de },
    { "el", &strings_el },
    { "es", &strings_es },
    { "fa", &strings_fa },
    { "fr", &strings_fr },
    { "he", &strings_he },
    { "hi", &strings_hi },
    { "hu", &strings_hu },
    { "it", &strings_it },
    { "ja", &strings_ja },
    { "km", &strings_km },
    { "ko", &strings_ko },
    { "pa", &strings_pa },
    { "pl", &strings_pl },
    { "ps", &strings_ps },
    { "pt", &strings_pt },
    { "ro", &strings_ro },
    { "ru", &strings_ru },
    { "sw", &strings_sw },
    { "ta", &strings_ta },
    { "th", &strings_th },
    { "tr", &strings_tr },
    { "uk", &strings_uk },
    { "ur", &strings_ur },
    { "vi", &strings_vi },
    { "zh", &strings_zh },
    { NULL, NULL },
};

/* Detect locale from environment (LC_ALL > LC_MESSAGES > LANG)
 * and return the matching string table, or English as fallback. */
const UchardetStrings* i18n_get_strings(void)
{
    const char* locale = getenv("LC_ALL");
    if (!locale || !*locale)
        locale = getenv("LC_MESSAGES");
    if (!locale || !*locale)
        locale = getenv("LANG");
    if (!locale || !*locale)
        return &strings_en;

    /* Match the first two characters (language code) against our table. */
    for (const LocaleEntry* e = locale_table; e->prefix; ++e)
    {
        if (locale[0] == e->prefix[0] && locale[1] == e->prefix[1])
            return e->strings;
    }

    return &strings_en;
}
