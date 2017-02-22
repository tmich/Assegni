#pragma once
#include <ostream>

namespace dtm
{
	enum month_t 
	{ 
		JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC 
	};
	
	enum weekday_t
	{
		SUN, MON, TUE, WED, THU, FRI, SAT
	};
	
	// ******************************
	// class date
	// ******************************
	class date
	{
	public:
		date();	// current date
		date(unsigned int day, unsigned int month, unsigned int year);	// specific date
		virtual ~date() {}

		unsigned char day() const { return m_day; }
		unsigned char month() const { return m_month; }
		unsigned short year() const { return m_year; }
		weekday_t weekday() const { return m_wday; }
		
		void setDay(unsigned char day);
		void setMonth(unsigned char month);
		void setYear(unsigned short year);
		
		dtm::date& operator++();
		dtm::date operator++(int);
		dtm::date& operator+=(int);
		bool operator==(const date& rhs);
		bool operator!=(const date& rhs);
		bool operator<(const date& rhs);
		bool operator>(const date& rhs);
		bool operator<=(const date& rhs);
		bool operator>=(const date& rhs);
	private:
		static bool isValid(unsigned char day, unsigned char month, unsigned short year);
		void addDays(unsigned char days);
		weekday_t calculateWeekday();
		unsigned int m_day, m_month, m_year;
		weekday_t m_wday;
	};

	// ******************************
	// class time
	// ******************************
	class time
	{
	public:
		time();		// current time
		time(unsigned char hh, unsigned char mm, unsigned char ss);

		unsigned char daylight() const { return m_daylight; }
		unsigned char hh() const { return m_hh; }
		unsigned char mm() const { return m_mm; }
		unsigned char ss() const { return m_ss; }
	private:
		unsigned char m_hh, m_mm, m_ss;
		unsigned char m_daylight;
	};
	
	// ******************************
	// class datetime
	// ******************************
	class datetime
	{
	public:
		datetime();		// current date-time
		datetime(unsigned char day, unsigned char month, unsigned short year, unsigned char hh = 0, unsigned char mm = 0, unsigned char ss = 0);

		date getDate() const { return m_date; }
		time getTime() const { return m_time; }

		unsigned char day() const { return m_date.day(); }
		unsigned char month() const { return m_date.month(); }
		unsigned short year() const { return m_date.year(); }
		weekday_t weekday() const { return m_date.weekday(); }

		unsigned char hh() const { return m_time.hh(); }
		unsigned char mm() const { return m_time.mm(); }
		unsigned char ss() const { return m_time.ss(); }

		void setDay(unsigned char day){ m_date.setDay(day); }
		void setMonth(char month){ m_date.setMonth(month); }
		void setYear(short year) { m_date.setYear(year); }

		//void addDays(unsigned int days){ m_date.addDays(days); }
	private:
		date m_date;
		time m_time;
	};

	// ******************************
	// interface parser
	// ******************************
	class parser
	{
	public:
		virtual datetime parse(std::wstring input) abstract;
	};

	// ******************************
	// helper Functions
	// ******************************
	unsigned int daysInMonth(int month, int year);
	bool isLeapYear(int year);
}

std::string strWeekday(const dtm::weekday_t & wday);
std::ostream& operator<< (std::ostream& stream, const dtm::date& date);
std::ostream& operator<< (std::ostream& stream, const dtm::time& time);
std::ostream& operator<< (std::ostream& stream, const dtm::datetime& datetime);
