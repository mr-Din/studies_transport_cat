# studies_transport_cat
Проект: Транспортный справочник (каталог)
<hr>
Функционал:<br>
- Получает и обрабатывает данные об остановках и маршрутах (через JSON).<br>
- По запросам выводит информацию о маршрутах, остановках (в форматах JSON и SVG).
<hr>
Использование:<br>
- Загрузить файлы проекта в среду разработки для сборки (использую VisualStudio, Eclipse).<br>
- Пример использования расположен в main.cpp. Входные данные расположены в input.json.<br>
- catalogue::json_reader::GetDataFromJson(std::istream& input) - формирует данные для последующего заполнения транспортного каталога и вывода информации.<br>
- catalogue::json_reader::FillTransportCatalogue(catalogue::TransportCatalogue& tc, DataFromJson& data_from_json) - заполняет транспортный каталог.<br>
- catalogue::json_reader::PrintStatInfo(TransportCatalogue& tc, DataFromJson& data_from_json, std::ostream& out) - выводит данные в формате JSON. Содержит в том числе данные для визуализации в формате SVG <br>
- При необходимости можно вывести только данные визуализации. Для этого создать объект renderer::MapRenderer с передачей в параметры каталога и render_settings из DataFromJson, вызвать метод установки значений SetRenderBus(), после вызвать метод Print(std::ostream& out).
<hr>
Системные требования:<br>
- C++17.
<hr>
Используемый стек:<br>
Функционал библиотеки STL, ООП, семантика перемещения.