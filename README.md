# htmlParser
# Алгоритм
1. С помощью библиотеки libcurl, отправляю запрос GET , подсунув мобильный user-agent, ловлю редирект, получаю html файл;//сразу убираю кучу ненужного
2. Определение кодировки (в комментах указано откуда взято);//по-хорошему, с самого html файла можно парсить
3. Если UTF-8, то работаем в рамках UnicodeString библиотеки ICU, если нет, то перед этим приводим к UTF-8;
4. Отделяю полезные блоки < p>...</ p>,<p ...>...</ p>, тэги заголовков
5. Преобразовываю ссылки, согласно правилу;
6. Удаляю лишние тэги, такие как < br>, < strong> и т.д., всякие спецсимволы типа &mdash и т.д.(можно их заменять);
7. Делю по 80 символов, согласно правилу;
8. Строю каталог по ссылке, записываю, каталог+text.txt.

# Результаты
Примеры данных сайтов, приведены в каталоге "examples";
https://lenta.ru/news/2019/07/02/deputat/;
https://www.gazeta.ru/science/news/2019/07/01/n_13157263.shtml.

# Улучшения
1. Надо сделать файл конфигурации, содержащий, удаляемые тэги, и заменяющие спецсимволы.
2. Оптимимзация, многопоточность.
3. Создавать файлы шаблонов для определенных сайтов, или по критериям, 
