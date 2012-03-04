/****************************************************************************
|*  string_conversions.h
|*
|*  Helpers to convert strings to different data types.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/



#ifndef _STRING_CONVERSIONS_H
#define _STRING_CONVERSIONS_H


#include <utils/Vector3.h>
#include <utils/Vector4.h>

#include <string>
#include <sstream>


// helper to convert strings to ints, floats or doubles
// the third parameter of from_string() should be 
// one of std::hex, std::dec or std::oct
template <class NumberType> bool stringToNumber(NumberType& t, const std::string& s, std::ios_base& (*f)(std::ios_base&) = std::dec) {
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}

// helper to convert a string with three whitespace-seperated numbers into a Vector3
template <class NumberType> bool stringToVector3(Vector3& vector3, std::string s){
	unsigned int i=0, j;
	NumberType number;
	i = static_cast<unsigned int>(s.find_first_not_of(std::string(" "),i));
	if (i==s.length()) {
		return false;
	}
	else {
		j=i;
		j=static_cast<unsigned int>(s.find_first_of(std::string(" "),j));
		if (!stringToNumber<NumberType>(number,s.substr(i,j-i))) {
			return false;
		}
		vector3.x = number;////////////////////////x
		i = j;
		i = static_cast<unsigned int>(s.find_first_not_of(std::string(" "),i));
		if (i==s.length()) {
			return false;
		}
		else {
			j=i;
			j=static_cast<unsigned int>(s.find_first_of(std::string(" "),j));
			if (!stringToNumber<NumberType>(number,s.substr(i,j-i))){
				return false;
			}
			vector3.y = number;////////////////////////y
			i = j;
			i = static_cast<unsigned int>(s.find_first_not_of(std::string(" "),i));
			if (i==s.length()) {
				return false;
			}
			else {
				j=i;
				j=static_cast<unsigned int>(s.find_first_of(std::string(" "),j));
				if (!stringToNumber<NumberType>(number,s.substr(i,j-i))) {
					return false;
				}
				vector3.z = number;////////////////////////z
			}
		}
	}

	return true;
}

// helper to convert a string with four whitespace-seperated numbers into a Vector4
template <class NumberType> bool stringToVector4(Vector4& vector4, std::string s){
	unsigned int i=0, j;
	NumberType number;
	i = static_cast<unsigned int>(s.find_first_not_of(std::string(" "),i));
	if (i==s.length()) {
		return false;
	}
	else {
		j=i;
		j=static_cast<unsigned int>(s.find_first_of(std::string(" "),j));
		if (!stringToNumber<NumberType>(number,s.substr(i,j-i))) {
			return false;
		}
		vector4.x = number;////////////////////////x
		i = j;
		i =static_cast<unsigned int>( s.find_first_not_of(std::string(" "),i));
		if (i==s.length()) {
			return false;
		}
		else {
			j=i;
			j=static_cast<unsigned int>(s.find_first_of(std::string(" "),j));
			if (!stringToNumber<NumberType>(number,s.substr(i,j-i))){
				return false;
			}
			vector4.y = number;////////////////////////y
			i = j;
			i =static_cast<unsigned int>( s.find_first_not_of(std::string(" "),i));
			if (i==s.length()) {
				return false;
			}
			else {
				j=i;
				j=static_cast<unsigned int>(s.find_first_of(std::string(" "),j));
				if (!stringToNumber<NumberType>(number,s.substr(i,j-i))) {
					return false;
				}
				vector4.z = number;////////////////////////z
				i = j;
				i =static_cast<unsigned int>( s.find_first_not_of(std::string(" "),i));
				if (i==s.length()) {
					return false;
				}
				else {
					j=i;
					j=static_cast<unsigned int>(s.find_first_of(std::string(" "),j));
					if (!stringToNumber<NumberType>(number,s.substr(i,j-i))) {
						return false;
					}
					vector4.w = number;////////////////////////w
				}
			}
		}
	}

	return true;
}


#endif //_STRING_CONVERSIONS_H
