#pragma once
#include <chrono>
#include <stdexcept>

class Date {
	private:
		std::chrono::year_month_day date_;

	public:
		Date(int year, unsigned int month, unsigned int day) 
			: date_{
				std::chrono::year{year}, 
					std::chrono::month{month}, 
					std::chrono::day{day}
			} {}

		std::chrono::year_month_day date() const {return date_;}

		std::chrono::sys_days sys_date() const {return date_;}

		int to_int(const Date* start_date) const {
			if (start_date == nullptr) {
				throw std::invalid_argument("A data de inicio nao pode ser nula.");
			}

			auto duration_days = sys_date() - start_date->sys_date();

			return static_cast<int>(duration_days.count())+1;
		}

		std::string to_string() const {
			return std::to_string(unsigned(date_.day()))   + "/" +
				std::to_string(unsigned(date_.month())) + "/" +
				std::to_string(int(date_.year()));
		}

		Date sum_days(const int days) const {
			auto result = sys_date() + std::chrono::days{days};
			auto ymd = std::chrono::year_month_day{result};
			return Date{int(ymd.year()), unsigned(ymd.month()), unsigned(ymd.day())};
		}

};
