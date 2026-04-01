#include <iostream> // Підключення бібліотеки для використання потоку вводу/виводу
#include "gameController.h" // Підключення заголовочного файлу, що забезпечує роботу з структурою GameController
#include <string> // Підключення бібліотеки для роботи з рядками
#include <conio.h> // Підключення бібліотеки для використання функції _getch()

void GameController::start_game() // Визначення функції, що відповідає за початок гри
{
	std::string user_input; // Оголошення змінної для збереження рядка

	while (true) // Нескінченний цикл
	{
		std::cout << "Оберіть розмір ігрового поля (від 3 до 9 включно): ";
		std::getline(std::cin, user_input); // Запис вводу користувача у рядок
		try // Блок обробки виключень
		{
			game_board.sizef = input_checking(user_input);
		}
		catch (std::invalid_argument& e) // Виняток перехвачується
		{
			std::cerr << "\nПомилку перехоплено!\nВведіть, будь ласка, ще раз.\nДеталі: " << e.what() << "\n\n";
			continue; // Початок циклу з початку
		}
		if (game_board.sizef < MinGameFieldSize || game_board.sizef > MaxGameFieldSize) // Інакше якщо введений розмір не задовольняє допустимий
		{
			std::cout << "\nВведений розмір ігрового поля не є допустимим!\n" << std::endl;
			continue; // Початок циклу з початку
		}
		break; // Вихід з циклу
	}
	move_count = 0; // Оновлення змінної, що підраховує кількість зроблених ходів
	game_board.create_board(game_board.sizef); // Запуск методу ініціалізації ігрового поля
	game_board.status = GameField::Active; // Встановлення статусу гри як активної
	game_loop(); // Запуск методу, що відповідає за основний цикл гри

	// Очищення пам'яті
	for (int i = 0; i < game_board.sizef; i++) // Цикл по рядкам, очищення пам'яті масивів рядків
	{
		delete[] game_board.board[i];
		game_board.board[i] = nullptr;
	}
	delete[] game_board.board; // Очищення пам'яті основного масиву
	game_board.board = nullptr;
}

int GameController::input_checking(std::string user_input)
{
	int number;
	size_t pos;
	try
	{
		number = stoi(user_input, &pos);
	}
	catch (const std::out_of_range&)
	{
		throw std::invalid_argument("\nВведене число є завеликим!");
	}
	catch (const std::invalid_argument&)
	{
		throw std::invalid_argument("\nВведений запис не є числом!");
	}
	if (pos < user_input.length())
	{
		throw std::invalid_argument("\nУ записі наявні зайві символи!");
	}
	return number;
}

void GameController::game_loop() // Визначення функції, що відповідає за логіку основного ігрового циклу
{
	std::string user_input; // Оголошення змінної для збереження рядка
	int token; // Оголошення змінної для збереження номера фішки


	while (game_board.status == GameField::Active) // Нескінченний цикл
	{
		std::cout << "Ігрове поле:\n" << std::endl;
		std::cout << game_board; // Друк ігрового поля на екран
		bool pause = false;
		while (true) // Нескінченний цикл
		{
			pause = false;
			std::cout << std::endl;
			std::cout << "Введіть номер фішки, яку бажаєте пересунути: ";
			std::getline(std::cin, user_input); // Запис вводу користувача у рядок
			for (int i = 0; i < user_input.length(); i++) // Цикл по кожному символу рядка
			{
				if (user_input[i] == 32) // Якщо натиснуто клавішу space
				{
					pause = true;
					break; // Вихід з циклу
				}
			}
			if (pause) // Якщо натиснуто паузу
			{
				game_board.status = GameField::Pause; // Встановлення статусу гри у режимі паузи
				std::cout << std::endl;
				std::cout << "Увікнуто режим паузи...\nНатисність Enter, щоб продовжити..." << std::endl;
				_getch(); // Очікування натискання клавіші
				game_board.status = GameField::Active; // Встановлення статусу гри у активному режимі 
				continue; // Початок циклу з початку
			}
			try
			{
				token = input_checking(user_input); // Конвертування рядка в числовий тип
				game_board >> token;
			}
			catch (const std::invalid_argument& e)
			{
				std::cerr << "\nПомилку перехоплено!\nВведіть, будь ласка, ще раз.\nДеталі: " << e.what() << "\nПерехід до нового ходу\n\n";
				break;
			}
			catch (std::logic_error& e)
			{
				std::cerr << "\nПомилку перехоплено!\nДеталі: " << e.what() << "Перехід до нового ходу\n\n";
				break;
			}

			move_count++; // Підрахунок кількості ходів
			std::cout << "Обрана фішка: " << token << std::endl;
			std::cout << std::endl;
			std::cout << "Хід виконано!" << std::endl;
			std::cout << std::endl;
			break; // Вихід з циклу
		}
		if (game_board.is_win()) // Якщо поле знаходиться у виграшному стані
		{
			std::cout << game_board;
			std::cout << std::endl;
			std::cout << "Вітаю з перемогою!\nГру завершено!\nКількість зроблених ходів: " << move_count << std::endl;
			std::cout << std::endl;
			game_board.status = GameField::Win; // Встановлення статусу гри як завершеної
		}
		continue; // Початок циклу з початку
	}
}