#ifndef _DECORATOR_H
#define _DECORATOR_H

#include "string"

using std::string;
namespace DP
{
	// Define the abstract Component
	class Beverage
	{
	public:
		Beverage()
		{
			m_strDescription = "Unknown beverage";
		}
		virtual double dbGetCost() = 0;
		virtual std::string strGetDescription()
		{
			return m_strDescription;
		}

	protected:
		// For subclass to access private description
		virtual void vSetDescription(std::string strDescription)
		{
			m_strDescription = strDescription;
		}

		//virtual void vSetDescription(const char* pstrDescription)
		//{
		//	std::string tempString = pstrDescription;
		//	m_strDescription = tempString;
		//}

	private:
		std::string m_strDescription;
	};


	// Define the abstract decorator which is derived from Component

	class CondimentDecorator: public Beverage
	{
	public:
		CondimentDecorator()
		{
		}
		
		// Override
		virtual std::string strGetDescription() = 0;
		virtual double dbGetCost() = 0;
	};

	// Define Concrete Component

	class CocaCola: public Beverage
	{
	public:
		CocaCola()
		{
			vSetDescription(std::string("CocaCola"));
		}

		virtual double dbGetCost()
		{
			return 1.4;
		}
	};

	class Coffee: public Beverage
	{
	public:
		Coffee()
		{
			vSetDescription(string("Coffee"));
		}

		virtual double dbGetCost()
		{
			return 1.8;
		}
	};

	// Define Concrete Decorator

	class Milk: public CondimentDecorator
	{
	public:
		Milk(Beverage* beverage)
		{
			m_poBeverageRef = beverage;
		}

		std::string strGetDescription()
		{
			 return m_poBeverageRef->strGetDescription() + " + Milk";
		}

		double dbGetCost()
		{
			return m_poBeverageRef->dbGetCost() + 1.2;
		}
	private:
		Beverage* m_poBeverageRef;
	};

	class Sugar: public CondimentDecorator
	{
	public:
		Sugar(Beverage* beverage)
		{
			m_poBeverageRef = beverage;
		}

		std::string strGetDescription()
		{
			return m_poBeverageRef->strGetDescription() + " + Sugar";
		}

		double dbGetCost()
		{
			return m_poBeverageRef->dbGetCost() + 1.5;
		}
	private:
		Beverage* m_poBeverageRef;
	};
}
#endif