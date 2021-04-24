Задание 1. (Rogue-like game)
Автор работы: Тверской А.Е. (313гр.)
Вариант: А3
Дата выполнения задания: 05.03.2021

Программа писалась под ОС Linux

Для запуска программы неоюходимо последовательно выполнить: 
	1)cmake .
	2)make
	3)cd ./bin
	4)./main

Управление персонажем осуществляется при помощи клавиш W,A,S,D.

Игра создавалась на компьютере с разрешением экрана 2560*1440, из-за чего
для удобства были выбраны тайлы размером 32*32; 
Размер окна игры 1600*1024 пикселя (50*32 = 1600 тайлов).

Список реализованных пунктов:
1)Базовая часть задания(вариант A3) 
  (реализация пола, по которому можно пройти огр. кол-во раз)	 
2)Реализация врагов, которые перемещаются по лабиринту (патрулируют по маршруту) 
3)Анимация статических объектов (подсвечивается выход из лабиринта)
4)Плавная спрайтовая анимация динамических объектов - походка игрока и
  врагов
5)Реализован графический эффект перехода между уровнями

Result:

![](https://github.com/sqrt-4/Study/blob/master/Rogue-like%20game/result.gif)