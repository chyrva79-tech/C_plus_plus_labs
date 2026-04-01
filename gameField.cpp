#include <iostream> // Підключення бібліотеки для забезпечення потокового вводу/виводу
#include "gameField.h" // Підключення створеного заголовочного файлу, що містить структуру GameField
#include "gameController.h"

void GameField::create_board(int size) // Визначення функції, що відповідає за початкову ініціалізацію ігрового поля
{
	sizef = size;
	board = new int*[size]; // Динамічне виділення пам'яті під масив рядків
	for (int i = 0; i < size; i++) // Цикл по рядкам
	{
		board[i] = new int[size]; // Динамічне виділення пам'яті для стовпців
	}
	int token_value = size * size - 1; // Прораховане значення останньої фішки
	for (int i = 0; i < size; i++) // Цикл по рядкам
	{
		for (int j = 0; j < size; j++) // Цикл по стовпцям
		{
			if (i == size - 1 && j == size - 1) // Якщо дана фішка є останньою
			{
				// Запис координат порожньої фішки
				coordinates.x = i;
				coordinates.y = j;
				board[i][j] = 0; // Ініціалізація значення порожньої фішки як нуль
				if (size % 2 == 0) // Якщо розмір поля парне число
				{
					// Зміна місцями елементів поля для забезпечення можливості виграшу
					int swap = board[i][j - 2];
					board[i][j - 2] = board[i][j - 1];
					board[i][j - 1] = swap;
				}
			}
			else // Якщо ж дана фішка не є останньою
			{
				board[i][j] = token_value; // Заповнення числом
				token_value--; // Зменшення даного числа на одну одиницю
			}
		}
	}
}

std::ostream& operator << (std::ostream& out, const GameField& var) // Перенавантаження оператора << для зручнішого виведення ігрового поля в консоль
{
	for (int i = 0; i < var.sizef; i++) // Цикл по рядкам
	{
		for (int j = 0; j < var.sizef; j++) // Цикл по стовпцям
		{
			if (i == var.coordinates.x && j == var.coordinates.y) // Якщо знайдено порожню клітинку
			{
				out << "_\t"; // Виводиться знак нижнього підкреслення
			}
			else // Якщо це звичайна клітинка
			{
				out << var.board[i][j] << "\t"; // Виводиться значення даної клітинки
			}
		}
		out << std::endl; // Відступ
	}
	return out;
}

bool GameField::is_move(int x, int y) // Визначення функції, що відповідає за перевірку можливості ходу
{
	if (sizef < MinGameFieldSize || sizef > MaxGameFieldSize) // Якщо розмір ігрового поля не відповідає заданим межам
	{
		return false;  
	}
	if ((abs(coordinates.x - x) == 1) && (coordinates.y == y) || (abs(coordinates.y - y) == 1) && (coordinates.x == x)) // Якщо фішка межує з порожньою клітинкою
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GameField::operator >> (int token) // Визначення перевизначеної функції, що відповідає за пересування фішки по ігровому полю
{
	int x = -1, y = -1; // Ініціалізація змінних, необхідних для знаходження координат фішки
	bool flag = false; // Ініціалізація змінної flag як false, необхідної для перевірки виконання умови

	if (token > sizef * sizef - 1 || token <= 0) // Якщо на вхід подається номер фішки, що виходить за межі ігрового поля
	{
		throw(std::logic_error("\nВи ввели номер фішки, що знаходиться поза межами ігрового поля!\nОберіть, будь ласка, іншу\n"));
	}
	for (int i = 0; i < sizef; i++) // Цикл по рядкам
	{
		for (int j = 0; j < sizef; j++) // Цикл по стовпцям
		{
			if (board[i][j] == token) // Якщо знайдено клітинку, що містить номер фішки
			{
				// Запис її координат
				x = i;
				y = j;
				flag = true; // Встановлення значення true для змінної
				break; // Вихід з внутрішнього циклу
			}
		}
		if (flag) // Якщо значення змінної flag є true
		{
			break; // Вихід з основного циклу
		}
	}

	if (is_move(x, y)) // Якщо хід можна здійснити
	{
		// Зміна місцями фішок 
		int tmp = board[x][y];
		board[x][y] = board[coordinates.x][coordinates.y];
		board[coordinates.x][coordinates.y] = tmp;
		// Оновлення координат порожньої клітинки
		coordinates.x = x;
		coordinates.y = y;

		return true;
	}
	else 
	{
		throw(std::logic_error("\nДана фішка є недоступною для руху!\nОберіть, будь ласка, іншу\n"));
	}
}

bool GameField::is_win() // Визначення функції, що відповідає за перевірку виграшу
{
	if (coordinates.x != sizef - 1 || coordinates.y != sizef - 1) // Якщо порожня клітинка знаходиться не на своєму місці
	{
		return false;
	}
	for (int i = 0; i < sizef; i++) // Цикл по рядкам
	{
		for (int j = 0; j < sizef; j++) // Цикл по стовпцям
		{
			if (i == sizef - 1 && j == sizef - 1) // Якщо натраплено на останню клітинку
			{
				continue; // Пропуск ітерації
			}
			if (board[i][j] != i * sizef + j + 1) // Якщо фішки не відсортовано по зростанню
			{
				return false;
			}
		}
	}
	return true;
}