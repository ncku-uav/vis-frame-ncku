/*
 * DecodingErrorNotifier.hpp
 *
 *  Created on: 30.04.2019
 *      Author: thibault
 */

#include <stdint.h>
#include "decoding_error_handler.hpp"

#ifndef INC_DECODINGERRORNOTIFIER_HPP_
#define INC_DECODINGERRORNOTIFIER_HPP_

class DecodingErrorNotifier : public DecodingErrorInterface{
public:
	DecodingErrorNotifier();
	~DecodingErrorNotifier();

	uint8_t error_detected = 0;
	uint16_t according_referenceId = 0;

	void notifyDecodingError(uint16_t referenceId) override;
	uint8_t get_error_status();

};

#endif /* INC_DECODINGERRORNOTIFIER_HPP_ */
