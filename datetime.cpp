#include "stdafx.h"
#include "datetime.h"
#include <stdexcept>
#include <ctime>
#include <string>
#include <map>
#include <iomanip>

// ******************************
// class date
// ******************************

bool dtm::date::isValid(unsigned char day, unsigned char month, unsigned short year)
{
	if (day < 1 || day > 31)
		return false;

	if (month < month_t::JAN || month > month_t::DEC)
		return false;

	if (day > 30 && (month == month_t::APR || month == month_t::JUN || month == month_t::SEP || month == month_t::NOV))
		return false;

	if (day == 29 && month == month_t::FEB)
		return isLeapYear(year);

	return true;
}

dtm::date::date()
	: m_day{ 1 }, m_month{ 1 }, m_year{ 1900 }
{
	__time64_t rawtime = std::time(nullptr);
	struct tm ptm;
	errno_t e = _gmtime64_s(&ptm, &rawtime);
	if (e != 0)
		throw std::exception("_gmtime64_s failed!");

	m_day = ptm.tm_mday;
	m_month = month_t(ptm.tm_mon + 1);
	m_year = ptm.tm_year + 1900;
	m_wday = weekday_t(ptm.tm_wday);
}


dtm::date::date(unsigned int day, unsigned int month, unsigned int year)
	: m_day{ 1 }, m_month{ 1 }, m_year{ 1900 }
{
	if (!dtm::date::isValid(day,month,year))
		throw std::out_of_range("date");
	
	m_day = day;
	m_month = month;
	m_year = year;
	m_wday = calculateWeekday();
}

void dtm::date::setDay(unsigned char day)
{
	if (!isValid(day, m_month, m_year))
		throw std::out_of_range("day");

	m_day = day;
	m_wday = calculateWeekday();
}

void dtm::date::setMonth(unsigned char month)
{
	if (!isValid(m_day, month, m_year))
		throw std::out_of_range("month");

	m_month = month;
	m_wday = calculateWeekday();
}

void dtm::date::setYear(unsigned short year)
{
	m_year = year;
	m_wday = calculateWeekday();
}

void dtm::date::addDays(unsigned char days)
{
	m_day += days;
	unsigned int maxDays = daysInMonth(m_month, m_year);
	
	while (m_day > maxDays)
	{
		m_day -= maxDays;

		if (m_month < month_t::DEC)
			m_month = month_t(m_month + 1);
		else
		{
			m_month = month_t::JAN;
			m_year += 1;
		}

		maxDays = daysInMonth(m_month, m_year);
	}

	m_wday = calculateWeekday();
}

dtm::weekday_t dtm::date::calculateWeekday()
{
	// https://en.wikipedia.org/wiki/Zeller%27s_congruence

	int q = m_day;						// the day of the month
	int m = m_month;					// the month(3 = March, 4 = April, 5 = May, ..., 14 = February)
	int Y = m_year;						// the year
	if (m == dtm::JAN || m == dtm::FEB)
	{
		m += 12;
		Y--;
	}

	int h = (q + ((13 * (m + 1)) / 5) + Y + (Y / 4) - (Y / 100) + (Y / 400)) % 7;	// (0 = Saturday, 1 = Sunday, 2 = Monday, ..., 6 = Friday)
	h = (h + 6) % 7;
	return dtm::weekday_t(h);
}

dtm::date& dtm::date::operator++() 
{ 
	addDays(1); 
	return *this;
}

dtm::date dtm::date::operator++(int)
{
	// postfix
	dtm::date tmp(*this);
	operator++();
	return tmp;
}

dtm::date& dtm::date::operator+=(int days)
{
	addDays(days);
	return *this;
}

bool dtm::date::operator==(const dtm::date& rhs)
{ 
	return m_day == rhs.m_day && m_month == rhs.m_month && m_year == rhs.m_year;
}

bool dtm::date::operator!=(const dtm::date& rhs)
{ 
	return !(*this == rhs); 
}

bool dtm::date::operator<(const dtm::date& rhs)
{
	if (*this == rhs)
		return false;

	if (m_year < rhs.m_year)
		return true;

	if (m_year == rhs.m_year)
	{
		if (m_month < rhs.m_month)
			return true;

		if (m_month == rhs.m_month)
			return m_day < rhs.m_day;
	}

	return false;
}

bool dtm::date::operator<=(const dtm::date& rhs)
{
	return (*this < rhs) || (*this == rhs);
}

bool dtm::date::operator>(const dtm::date& rhs)
{
	return (*this != rhs) && !(*this < rhs);
}

bool dtm::date::operator>=(const dtm::date& rhs)
{
	return (*this > rhs) || (*this == rhs);
}

// ******************************
// class time
// ******************************
dtm::time::time()
{
	__time64_t rawtime = std::time(nullptr);
	struct tm ptm;
	errno_t e = _gmtime64_s(&ptm, &rawtime);
	if (e != 0)
		throw std::exception("_gmtime64_s failed!");

	int daylight = 0;
	errno_t de = _get_daylight(&daylight);
	if (de != 0)
		throw std::exception("_get_daylight failed!");

	m_daylight = daylight;
	m_hh = ptm.tm_hour + daylight;
	m_mm = ptm.tm_min;
	m_ss = ptm.tm_sec;
}

dtm::time::time(unsigned char hh, unsigned char mm, unsigned char ss)
	: m_hh{ 0 }, m_mm{ 0 }, m_ss{ 0 }
{
	if (hh >= 0 && hh < 24)
		m_hh = hh;

	if (mm >= 0 && mm < 60)
		m_mm = mm;

	if (ss >= 0 && ss < 60)
		m_ss = ss;
}

// ******************************
//	class datetime
// ******************************
dtm::datetime::datetime()
{
}

dtm::datetime::datetime(unsigned char day, unsigned char month, unsigned short year, 
						unsigned char hh, unsigned char mm, unsigned char ss)
	: m_date{ day, month, year }, m_time{ hh, mm, ss }
{
}

// ******************************
// helper Functions
// ******************************

bool dtm::isLeapYear(int year)
{
	// sono bisestili:
	// gli anni non secolari il cui numero è divisibile per 4
	if (year % 100 != 0 && year % 4 == 0)
		return true;

	// gli anni secolari il cui numero è divisibile per 400
	if (year % 100 == 0 && year % 400 == 0)
		return true;

	return false;
}

unsigned int dtm::daysInMonth(int month, int year)
{
	switch (month)
	{
	case dtm::FEB:
		if (isLeapYear(year))
			return 29;
		else
			return 28;
	case dtm::JAN:
	case dtm::MAR:
	case dtm::MAY:
	case dtm::JUL:
	case dtm::AUG:
	case dtm::OCT:
	case dtm::DEC:
		return 31;
	default:
		return 30;
	}
}

std::string strWeekday(const dtm::weekday_t & wday)
{
	std::string weekDay;

	switch (wday)
	{
	case dtm::weekday_t::SUN:
		weekDay = "Domenica";
		break;
	case dtm::weekday_t::MON:
		weekDay = "Lunedi";
		break;
	case dtm::weekday_t::TUE:
		weekDay = "Martedi";
		break;
	case dtm::weekday_t::WED:
		weekDay = "Mercoledi";
		break;
	case dtm::weekday_t::THU:
		weekDay = "Giovedi";
		break;
	case dtm::weekday_t::FRI:
		weekDay = "Venerdi";
		break;
	case dtm::weekday_t::SAT:
		weekDay = "Sabato";
		break;
	default:
		throw std::out_of_range("weekday");
	}

	return weekDay;
}


std::ostream& operator<< (std::ostream& stream, const dtm::date& date)
{
	stream << std::setfill('0') << strWeekday(date.weekday()) << " " << std::setw(2) << std::to_string(date.day()) << "\\" << std::setw(2) << std::to_string(static_cast<int>(date.month())) << "\\" << std::to_string(date.year());
	return stream;
}

std::ostream& operator<< (std::ostream& stream, const dtm::time& time)
{
	stream << std::setfill('0') << std::setw(2) << std::to_string(time.hh()) << ":" << std::setw(2) << std::to_string(time.mm()) << ":" << std::setw(2) << std::to_string(time.ss());
	return stream;
}

std::ostream& operator<< (std::ostream& stream, const dtm::datetime& datetime)
{
	stream << datetime.getDate() << " " << datetime.getTime();
	return stream;
}