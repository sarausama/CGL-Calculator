#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK
//
 Transaction::Transaction (std::string ticker_symbol, unsigned int day_date,
              unsigned int month_date, unsigned year_date, bool buy_sell_trans,
              unsigned int number_shares, double trans_amount) :
    day {day_date},
    month {month_date},
	year {year_date},
	shares {number_shares},
	amount {trans_amount},
	p_next {nullptr},
	acb{0},
	cgl{0},
	acb_per_share{0},
	share_balance{0}
 {
	  symbol = ticker_symbol;
	  if(buy_sell_trans){
		  trans_type = "Buy";
	  } else{
		  trans_type = "Sell";
	  }
	  trans_id = assigned_trans_id;
	  ++ assigned_trans_id;
 }


// Destructor
// TASK
//

 Transaction::~Transaction()
 {
	  p_next = nullptr;
 }

// TASK
// Overloaded < operator.
//
 bool Transaction::operator<(Transaction const &other) {
	  if(year < other.get_year()){
		  return true;
	  } else if(year == other.get_year()){
		  if(month <other.get_month()){
			  return true;
		  } else if(month == other.get_month()){
			  if(day<other.get_day()){
				  return true;
			  } else {
				  return false;
			  }
		  }else {
			  return false;
		  }
	  } else {
		  return false;
	  }
	  return false;
 }
// GIVEN
// Member functions to get values. 
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values. 
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " " 
    << std::setw(4) << get_symbol() << " " 
    << std::setw(4) << get_day() << " "  
    << std::setw(4) << get_month() << " " 
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) { 
    std::cout << "  Buy  "; 
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " " 
    << std::setw(10) << get_amount() << " " 
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl() 
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK
//
History::History():
	 p_head { nullptr}
{}

// Destructor
// TASK
//
History::~History() {
while (p_head != nullptr){
	  Transaction *temp = p_head->get_next();
	  delete p_head;
	  p_head = temp;
}
}

// TASK
// read_transaction(...): Read the transaction history from file. 
//
void History::read_history() {
	  ece150::open_file();

	  while (ece150::next_trans_entry()){
		  History::insert(new Transaction(ece150::get_trans_symbol(),
				  ece150::get_trans_day(), ece150::get_trans_month()
				  , ece150::get_trans_year(), ece150::get_trans_type(),
				  ece150::get_trans_shares(), ece150::get_trans_amount()));
	  }
	  ece150::close_file();
}

// insert(...): Insert transaction into linked list.
//
void History::insert(Transaction *p_new_trans) {
	  Transaction* temp = p_head;
	  if(p_head != nullptr){
	  while (temp->get_next() != nullptr){
		  temp = temp->get_next();
	  }
	  temp->set_next(p_new_trans);
	  } else{
		  p_head = p_new_trans;
	  }
}


// TASK
// sort_by_date(): Sort the linked list by trade date.
//

void History::sort_by_date(){
	Transaction* counter {p_head};
	bool flag {true};
if(p_head != nullptr){
	while(counter->get_next() != nullptr){
		flag = true;
		if(*(counter->get_next()) < *p_head){
			Transaction* temp {counter->get_next()};
			counter->set_next((counter->get_next())->get_next());
			temp->set_next(p_head);
			p_head = temp;
			flag = false;
		} else {
			Transaction* start{p_head};
			while(start->get_next() != counter->get_next()){
				if(*(counter->get_next()) < *(start->get_next())){
					Transaction* temp{counter->get_next()};
					counter->set_next((counter->get_next())->get_next());
					temp->set_next(start->get_next());
					start->set_next(temp);
					flag = false;
					break;
				}
				start = start->get_next();
			}
		}
		if (flag == true)
			counter = counter->get_next();
	}
}
}

// TASK
// update_acb_cgl(): Updates the ACB and CGL values. 
//

void History::update_acb_cgl( ){
	if(p_head != nullptr){
	Transaction* temp{p_head};
	p_head->set_acb((p_head->get_amount()));
	p_head->set_share_balance(p_head->get_shares());
	p_head->set_acb_per_share((p_head->get_acb())/p_head->get_shares());
	while (temp->get_next()!=nullptr){
		if((temp->get_next())->get_trans_type() == true){
		(temp->get_next())->set_acb((temp->get_next()->get_amount()) + temp->get_acb());
		(temp->get_next())->set_share_balance((temp->get_next()->get_shares()) + temp->get_share_balance());
		(temp->get_next())->set_acb_per_share(((temp->get_next())->get_acb())/((temp->get_next())->get_share_balance()));
		} else {
			(temp->get_next())->set_share_balance(temp->get_share_balance() - (temp->get_next()->get_shares()));
			(temp->get_next())->set_acb_per_share(temp->get_acb_per_share());
			(temp->get_next())->set_acb(temp->get_acb() - ((temp->get_next())->get_shares()) * (temp->get_acb_per_share()));
			(temp->get_next())->set_cgl((temp->get_next())->get_amount() - (temp->get_acb_per_share())* ((temp->get_next())->get_shares()));
		}
		temp = temp->get_next();
	}
	}
}

// TASK
// compute_cgl(): )Compute the ACB, and CGL.
//

double History::compute_cgl( unsigned int year ){
	double cgl{0};
	Transaction* temp{p_head};
	if(temp != nullptr){
	while(temp->get_year() != year){
		temp = temp->get_next();
	}
	while((temp!= nullptr) && (temp->get_year() == year)){
		cgl = cgl + temp->get_cgl();
		temp = temp->get_next();
	}
	return cgl;
	}
	return 0;
}


// TASK
// print() Print the transaction history.
//
void History::print(){
	Transaction* temp{p_head};
	std::cout<<"========== BEGIN TRANSACTION HISTORY ============"<<std::endl;
	while(temp != nullptr){
		temp->print();
		temp=temp->get_next();
	}
	std::cout<<"========== END TRANSACTION HISTORY ============"<<std::endl;
}



// GIVEN
// get_p_head(): Full access to the linked list.
// 
Transaction *History::get_p_head() { return p_head; }


#endif

