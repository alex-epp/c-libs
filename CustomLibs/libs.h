#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace CustomLibs
{

	namespace exception
	{
		//class 
	}

	namespace input
	{
		template <typename T = int, typename F = function<bool(T)> >
		T get_num(std::string prompt, F func = [](T x) {return true; }, bool allow_default = false, T default_value = 0)
		{
			while (true)
			{
				try
				{
					std::cout << prompt;

					std::string line;
					std::getline(std::cin, line);

					if (line.empty())
					{
						if (!allow_default)
							continue;
						else
							return default_value;
					}


					std::stringstream ss(line);
					int64_t val;
					ss >> val;

					if (!ss.fail())
						if (val == static_cast<int64_t>(static_cast<T>(val)))
							if (func(val))
								return val;
				}
				catch (...) {}
			}
		}
	}

	namespace binary
	{
		template <typename T>
		std::string to_binary(T a)
		{
			if (a < 0)
				return "";

			std::string s;
			while (a != 0)
			{
				s += (a & 1) ? "1" : "0";
				a = a >> 1;
			}

			if (s == "")
				s = "0";

			std::reverse(s.begin(), s.end());

			return s;
		}

		template <typename T>
		T to_decimal(std::string bin)
		{
			if (bin.empty())
				return 0;

			T d = 0;
			std::reverse(bin.begin(), bin.end());
			T i = 1;
			for (auto b : bin)
			{
				if (b == '1')
					d += i;
				else if (b != '0')
					throw std::invalid_argument("to_decimal takes a string with only '0's and '1's.");

				i *= 2;
			}

			return d;
		}
	}

	namespace profile
	{
		template <typename T, typename S, typename A, typename B, typename C>
		void profile3(T function, int iter1, int iter2, S& out, A f_a, B f_b, C f_c)
		{
			out << "A, B, C, Elapsed Time\n";

			for (auto i = 0; i < iter1; ++i)
			{
				auto a = f_a(i);
				auto b = f_b(i);
				auto c = f_c(i);

				std::chrono::time_point<std::chrono::system_clock> start, end;
				start = std::chrono::system_clock::now();
				for (auto j = 0; j < iter2; ++j)
					function(a, b, c);
				end = std::chrono::system_clock::now();
				std::chrono::duration<double> elapsed_seconds = end - start;

				out << a << ", " << b << ", " << c << ", " << elapsed_seconds.count();
				out << std::endl;
			}
		}
		template <typename T, typename S, typename A, typename B>
		void profile2(T function, int iter1, int iter2, S& out, A f_a, B f_b)
		{
			out << "A, B, Elapsed Time\n";

			for (auto i = 0; i < iter1; ++i)
			{
				auto a = f_a(i);
				auto b = f_b(i);

				std::chrono::time_point<std::chrono::system_clock> start, end;
				start = std::chrono::system_clock::now();
				for (auto j = 0; j < iter2; ++j)
					function(a, b);
				end = std::chrono::system_clock::now();
				std::chrono::duration<double> elapsed_seconds = end - start;

				out << a << ", " << b << ", " << elapsed_seconds.count();
				out << std::endl;
			}
		}
		template <typename T, typename S, typename A>
		void profile1(T function, int iter1, int iter2, S& out, A f_a)
		{
			out << "A, Elapsed Time\n";

			for (auto i = 0; i < iter1; ++i)
			{
				auto a = f_a(i);

				std::chrono::time_point<std::chrono::system_clock> start, end;
				start = std::chrono::system_clock::now();
				for (auto j = 0; j < iter2; ++j)
					function(a);
				end = std::chrono::system_clock::now();
				std::chrono::duration<double> elapsed_seconds = end - start;

				out << a << ", " << elapsed_seconds.count();
				out << std::endl;
			}
		}
	}

	namespace IO
	{
		class OStreamTuple
		{
		public:
			template <typename... Args>
			OStreamTuple(Args&&... args) : streams{ std::forward<Args>(args)... } {}

			template <typename T, typename... Args>
			OStreamTuple& add(T&& first, Args&&... next)
			{
				streams.emplace_back(std::forward<T>(first));
				return add(std::forward<Args>(next)...);
			}
			template <typename T>
			OStreamTuple& add(T&& first)
			{
				streams.emplace_back(std::forward<T>(first));
				return *this;
			}

			template <typename T>
			OStreamTuple& operator << (T&& rhs)
			{
				for (auto s : streams)
					(*s) << std::forward<T>(rhs);
				return *this;
			}
			OStreamTuple& operator << (const char* rhs)
			{
				for (auto s : streams)
					(*s) << rhs;
				return *this;
			}
			OStreamTuple& operator<<(std::ostream&(*pf)(std::ostream&))
			{
				for (auto s : streams)
					(*s) << *pf;
				return *this;
			}

		private:
			std::vector<std::ostream*> streams;
		};
	}
}