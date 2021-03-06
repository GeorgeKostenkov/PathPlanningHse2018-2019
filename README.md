# PathPlanning
## Назначение программы
Данная программа предназначена для нахождения кратчайшего пути из заданного пункта А в заданный пункт B на плоскости с расположенными на ней объектами-препятствиями. На вход ей подается файл формата XML и на выход получаем файл того же формата XML.
## Сборка проекта
   1. В командной строке в каталоге с вашим проектом напишите следующее:
      qmake -o Makefile * название проекта (в данный момент ASearch) *.pro
   2. Запустите make, написав make в коммандной строке.
   На выходе получаете исполняемый файл с названием нашего проекта (ASearch).

## Входные данные
### Параметры карты.
   1. Ширина
   2. Высота
   3. Размер клетки
   4. Координаты точки старта по оси ОХ.
   5. Координаты точки старта по оси ОY.
   6. Координаты точки финиша по оси ОХ.
   7. Координаты точки финиша по оси ОY.
   8. Сама Карта (в виде сетки).
   Пример такой сетки: 
   ```
   0 0 0 0
   1 1 0 0
   1 1 1 0
   0 0 0 1
   ```
### Параметры алгоритма.
   1. Тип поиска
   2. Тип метрики
   3. breakingties - предпочтение в равной ситуации
   4. hweight - вес выбранной эвристики
   5. Разрешено ли перемещение по диагонали
   6. Разрешено ли срезать углы
   7. Разрешен ли путь через щель между препятствиями (требуется выполнение условия 6)
### Опции отчёта.
   1. Уровень
   2. Путь к отчёту
   3. Имя файла с отчётомм
## Выходные данные (отчёт).
   ### Первая часть = Входные данные
   ### Сам отчет
   1. Суммарное время, масшабированная длина, длина, число созданных узлов, число шагов.
   2. Сам путь на карте (сетке). Пример:
   ```
   0 * 0 0
   1 1 * 0
   1 1 1 *
   0 * * 1
   ```
   3. Узлы сетки, через которые проходит путь (номер, координата по у, координата по х)
   4. Участки (участок меняется при изменении направления движения) этого пути (длина участка, номер участка, координата финиша участка по х и у, координата старта участка по х и у)
   
   Таким образом, мы и находим кратчайший путь между двумя данными вершинами на плоскости.

Проект выполнен студентом НИУ ВШЭ ФКН ПМИ Костенковым Георгием.
