/*
 * BIT_MATH.h
 *
 *  Created on: Nov 7, 2022
 *      Author: Alaa
 */

#ifndef LIBRARY_BIT_MATH_H_
#define LIBRARY_BIT_MATH_H_

#define SET(reg,bit)  		 (reg |= (1<<bit))
#define CLEAR(reg,bit)   	 (reg &= ~(1<<bit))
#define TOGGLE(reg,bit)   	 (reg ^= (1<<bit))
#define GET(REG,BIT)  		 ((REG >> BIT ) &0x01)



#endif
