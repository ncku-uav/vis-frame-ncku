/*
 * DecodingErrorNotifier.cpp
 *
 *  Created on: 30.04.2019
 *      Author: thibault
 */

#include <DecodingErrorNotifier.hpp>

DecodingErrorNotifier::DecodingErrorNotifier() {
	// TODO Auto-generated constructor stub

}

DecodingErrorNotifier::~DecodingErrorNotifier() {
	// TODO Auto-generated destructor stub
}

void DecodingErrorNotifier::notifyDecodingError(uint16_t referenceId)
{
	according_referenceId = referenceId;
	error_detected = 1;
}

uint8_t DecodingErrorNotifier::get_error_status()
{
	switch(error_detected)
	{
	case 1:
		error_detected = 0;
		return 1;
		break;
	case 0:
		error_detected = 0;
		return 0;
		break;
	}
	return 1;
}

