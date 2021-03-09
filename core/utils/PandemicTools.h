/*
 * PandemicTools.h
 *
 *  Created on: 22 Sep 2020
 *      Author: sam
 */

#ifndef CORE_UTILS_PANDEMICTOOLS_H_
#define CORE_UTILS_PANDEMICTOOLS_H_

#include <iostream>
#include <unordered_map>


	typedef std::unordered_map<unsigned long, bool> LogEntry;
	typedef std::unordered_map<int, LogEntry> InfectionLog;
	typedef std::unordered_map<unsigned long, unsigned long> NetworkNode;
	typedef std::unordered_map<unsigned long, NetworkNode> Network;

//template<typename T>
//void write_network(std::ostream& output, T input) {
//}


void write_network(std::ostream& output, Network input) {
	output << "FROM,TO,WEIGHT\n";
	for (auto& level_1 : input)
		for (auto& level_2 : level_1.second)
			output << level_1.first << ',' << level_2.first << ',' << level_2.second << '\n';
}

void write_report(std::ostream& output, InfectionLog input) {
	output << "date,uid,value\n";
	for (auto& level_1 : input)
		for (auto& level_2 : level_1.second)
			output << level_1.first << ',' << level_2.first << ',' << level_2.second << '\n';
}



#endif /* CORE_UTILS_PANDEMICTOOLS_H_ */
