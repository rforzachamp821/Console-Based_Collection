
// CalculationAlgorithm - A numerical-only calculator class with support for up to a theoretical 18 trillion characters per calculation line.
//
// It supports the modern implementation of the BIDMAS order of operations, negative numbers, spaces and is crash-safe.
// It also supports multipliers after brackets (without the multiplication symbol also works), powers, and support for multiple
// addition-subtraction operators beside each other. It also supports multiple brackets inside of each other.
// Basic trigonometric and arc-trigonometric functions are available and supported. 2 constants, PI and E (Euler's Number), are supported
// and calculated at runtime as well.
// The CalculationAlgorithm also supports square and cube root functions. The algorithm is NOT case-sensitive.
// 
// The allowed operators are: (), *, /, +, -, and ^ (power of). You must have a number on each side of the operators.
// 
// Example allowed expressions are:
// -> 5+5(5)
// -> 7*4+(6/2)
// -> 5+++-+---+-6*7/5(3)4
// -> 5^2+9/3
// -> (10+5^2)((5*-2)+9-3^3)/2
// -> 3(sin(5)acos(-1))
//
// - This calculation algorithm is currently limited by the maximum precision of long double (16 digits of precision). 
// 
// - Please beware that the number outputted may not actually be 16 digits and possibly less, due to natural IEE754 limits.
// 
// - Please note that there is no support for logarithmic functions yet.
//
class CalculationAlgorithm 
{
protected:

	// Constants - Pi and Degrees -> Radians Calculation Constant (and opposite)
	const long double dPiConstant = std::acosl(-1);
	const long double dEulerConstant = std::expl(1.0);
	const long double dDegToRad = dPiConstant / 180;
	const long double dRadToDeg = 180 / dPiConstant;

	const char        cDefaultDecimalPoint = '.';
	const char        cDefaultThousandsSeparator = ',';
	const char        cEuropeanDecimalPoint = ',';
	const char        cEuropeanThousandsSeparator = '.';

	char              cDecimalPoint = '.';
	char              cThousandsSeparator = ',';


	// isNumber - Checks whether std::string argument is a true number or not
	// Arguments: sNumberTest - The number string to check.
	// Return Values: TRUE or 1 for number, FALSE or 0 for not a number.
	//
	bool isNumber(const std::string sNumberTest) {

		// Variables
		bool bDecimalPointAlreadyFound = false;

		// Not a number as there's nothing in the string
		if (sNumberTest.length() <= 0) return false;

		// Check if all digits are numbers
		for (size_t i = 0; i < sNumberTest.length(); i++) {
			if (i == 0) {
				// negative number
				if (sNumberTest[0] == '-') continue;
			}

			// skip character if it's a decimal point
			if (sNumberTest[i] == cDefaultDecimalPoint && bDecimalPointAlreadyFound == false) {
				// Allow it only once
				bDecimalPointAlreadyFound = true;
				continue;
			}
			else if (isdigit(sNumberTest[i]) == false) return false;
		}

		// Test if above stold() range
		try {
			long double nRangeTest = std::stold(sNumberTest);
		}
		catch (const std::out_of_range& oorIsNumber) {
			UserErrorDisplay("Exception caught - Number is too high/low (out of range).");
			return false;
		}

		return true;
	}

	// FormatValueForDisplay - Formats a specific value for display output.
	//                       - Has a max of 15-16dp.
	// Arguments: dValue - The number to format.
	// Return Value: Formatted number as a string.
	//
	std::string FormatValueForDisplay(long double dValue) {
		// Create stringstream and send info to it
		std::stringstream ssFormatted;
		ssFormatted << std::fixed << std::setprecision(std::numeric_limits<long double>::digits10 - GetWholeNumberDigitLength(dValue)) << dValue;

		std::string sFormatBuffer = EradicateTrailingZeroes(ssFormatted.str());

		// Change decimal point to European notation if necessary
		//
		// Find last decimal point and replace with european one
		size_t nDecimalPointPos = sFormatBuffer.find_last_of(cDefaultDecimalPoint);

		// Exit when not found
		if (nDecimalPointPos == std::string::npos) return sFormatBuffer;

		// Replace
		sFormatBuffer.replace(nDecimalPointPos, 1, std::string(1, cDecimalPoint));

		// Return string from stringstream
		return sFormatBuffer;
	}

	// FormatValueForAlgorithm - Formats a specific value for internal algorithm calculations.
	//                         - Has a max of 18-19dp.
	// Arguments: dValue - The number to format.
	// Return Value: Formatted number as a string.
	//
	std::string FormatValueForAlgorithm(long double dValue) {
		// Create stringstream and send info to it
		std::stringstream ssFormatted;
		ssFormatted << std::fixed << std::setprecision(std::numeric_limits<long double>::max_digits10 + 1) << dValue;

		// Return string from stringstream
		return ssFormatted.str();
	}

	// FormatStringForDisplay - Formats a calculation string for display output, by rounding the numbers inside and removing the zeroes from them.
	// Arguments: sCalculationString - The string to format.
	// Return value: Formatted string.
	//
	std::string FormatStringForDisplay(std::string sCalculationString) 
	{
		// 1. Get calculation string and declare for loop, variables
		size_t nLastEndNumberPos = 0;
		while (true) 
		{
			// 2. Search from left to right, for first numerical character (ignore '-'). Exit loop on std::string::npos
			size_t nNumberPosStart = nLastEndNumberPos;
			for (; nNumberPosStart < sCalculationString.length(); nNumberPosStart++) {
				// Number exits the loop
				if (isdigit(sCalculationString[nNumberPosStart])) {
					break;
				}
			}

			// Return on completion
			if (nNumberPosStart >= sCalculationString.length()) return sCalculationString;

			// 3. Using for loop, loop to end of number by checking numerical characters (ignore '.')
			size_t nNumberPosEnd = nNumberPosStart;
			for (; nNumberPosEnd < sCalculationString.length(); nNumberPosEnd++) {
				// Non-number exits the loop
				if (!isdigit(sCalculationString[nNumberPosEnd]) && sCalculationString[nNumberPosEnd] != '.') {
					break;
				}
			}

			// Return on completion
			if (nNumberPosEnd > sCalculationString.length()) return sCalculationString;

			// 4. Parse string from both points, and convert to number
			long double dFormatBuffer = 0.0;
			std::string sFormatBuffer = sCalculationString.substr(nNumberPosStart, (nNumberPosEnd - nNumberPosStart));

			// 5. Convert string to a number, with type checking beforehand
			if (!isNumber(sFormatBuffer)) {
				nErrorLevel = 2;
				return sCalculationString;
			}

			// Convert string to number
			dFormatBuffer = std::stold(sFormatBuffer);

			// 6. Pass number to custom formatting function, and replace contents of string's number with new, display-formatted number string
			size_t nValueLength = FormatValueForDisplay(dFormatBuffer).length();
			sCalculationString.replace(nNumberPosStart, (nNumberPosEnd - nNumberPosStart), FormatValueForDisplay(dFormatBuffer));

			// Set last ended number position to start pos + nValueLength
			nLastEndNumberPos = nNumberPosStart + nValueLength;
		}

		return sCalculationString;
	}
	
	// FormatStringForAlgorithm - Formats a calculation string for algorithm use, including changing number formatting to the American notation.
	// Arguments: sCalculationString - The string to format.
	// Return value: Formatted string.
	//
	std::string FormatStringForAlgorithm(std::string sCalculationString) 
	{
		while (true) {
			// Change decimal notation to American
			size_t nDecimalPointLocation = sCalculationString.find_last_of(cEuropeanDecimalPoint);

			// Exit when not found
			if (nDecimalPointLocation == std::string::npos) {
				break;
			}

			// Replace
			sCalculationString.replace(nDecimalPointLocation, 1, std::string(1, cDefaultDecimalPoint));
		}

		return sCalculationString;
	}

	// nErrorLevel - The type of error that occured when calculating. The following values are what they are:
	//              0) No error has occured.
	//              1) A math error has occured (e.g division by 0).
	//              2) A syntax error has occured.
	//              3) An unknown number parsing error has occured.
	//              4) The calculated number is either infinity or too large to calculate.
	// This can be recieved as a string using the CalculationAlgorithm::GetLastCalculationErrorInfo() function.
	//
	short int nErrorLevel = 0;

	// dLastAns - The last calculated value known on the object.
	//          - Used for the (Ans) user-space variable.
	//
	long double dLastAns = 0;


	// Calculate - Main calculation algorithm that calculates using each of the 4 basic mathematical operators,
	//             with negative number support along with that. The nErrorLevel variable is modified when 
	//             anything goes wrong.
	// Arguments: sCalculationString - The calculation string to work out the product of/find the answer of.
	// Return values: The calculated answer in long double-grade precision.
	//
	long double Calculate(std::string sCalculationString) {

		long double dFinalResult = 0;

		// Exit immediately on pi / e finding
		if (sCalculationString == "pi" || sCalculationString == "e" || sCalculationString == "ans") {
			return 0.0;
		}

		// Display first calculation if working out is allowed
		if (bDisplayWorkingOutProcess) std::cout << "--> " << FormatStringForDisplay(sCalculationString) << '\n';


		// 1. Powers
		while (true)
		{
			size_t nSymbolLocation = 0;

			// Check for any unnecessary arithmetic symbols before the run
			RemoveUnnecessaryArithmeticSymbols(sCalculationString);

			// a. Find multiplication symbol (*) and get location; if not found, break
			nSymbolLocation = sCalculationString.find("^", 0);
			if (nSymbolLocation == std::string::npos) break;

			// b. Check if both sides of the power symbol are numbers and the symbol isn't at the start/end of string
			//    - if all true, continue. If not, post syntax error and exit
			if (nSymbolLocation > 0 && nSymbolLocation < sCalculationString.length() - 1) {
				if (std::isdigit(sCalculationString[nSymbolLocation - 1]) == false) {
					nErrorLevel = 2;
					dFinalResult = 0;
					return dFinalResult;
				}
				if (std::isdigit(sCalculationString[nSymbolLocation + 1]) == false && sCalculationString[nSymbolLocation + 1] != '-') {
					nErrorLevel = 2;
					dFinalResult = 0;
					return dFinalResult;
				}
				else if (sCalculationString[nSymbolLocation + 1] == '-') {
					if (nSymbolLocation + 1 < sCalculationString.length()) {
						if (std::isdigit(sCalculationString[nSymbolLocation + 2]) == false) {
							nErrorLevel = 2;
							dFinalResult = 0;
							return dFinalResult;
						}
					}
					else {
						nErrorLevel = 2;
						dFinalResult = 0;
						return dFinalResult;
					}
				}
			}
			else {
				nErrorLevel = 2;
				dFinalResult = 0;
				return dFinalResult;
			}

			// c. Check for first number location before power sign
			size_t nNumber1StartLocation = nSymbolLocation - 1;
			for (; nNumber1StartLocation >= 0; nNumber1StartLocation--) {
				if (isdigit(sCalculationString[nNumber1StartLocation]) == false && sCalculationString[nNumber1StartLocation] != cDefaultDecimalPoint) { // decimal point accepted
					nNumber1StartLocation++; // to avoid the non-numerical character
					break; // Start of number found
				}
				else if (nNumber1StartLocation <= 0) {
					break; // Start of number found
				}
			}

			// d. Check for second number location after power sign
			size_t nNumber2EndLocation = nSymbolLocation + 1;
			for (; nNumber2EndLocation <= sCalculationString.length() - 1; nNumber2EndLocation++) {
				if (nNumber2EndLocation == nSymbolLocation + 1) {
					if (sCalculationString[nNumber2EndLocation] == '-') {
						// continue as this should be a negative number
						continue;
					}
				}
				if (isdigit(sCalculationString[nNumber2EndLocation]) == false && sCalculationString[nNumber2EndLocation] != cDefaultDecimalPoint) { // decimal point accepted
					nNumber2EndLocation--; // to avoid the non-numerical character
					break; // Start of number found
				}
				else if (nNumber2EndLocation >= sCalculationString.length() - 1) {
					break; // Start of number found
				}
			}

			// e. Convert strings into numbers with checks
			std::string sNumber1 = sCalculationString.substr(nNumber1StartLocation, nSymbolLocation - nNumber1StartLocation);
			std::string sNumber2 = sCalculationString.substr(nSymbolLocation + 1, nNumber2EndLocation - nSymbolLocation);

			if (isNumber(sNumber1) == false) {
				nErrorLevel = 3;
				dFinalResult = 0;
				return dFinalResult;
			}
			if (isNumber(sNumber2) == false) {
				nErrorLevel = 3;
				dFinalResult = 0;
				return dFinalResult;
			}

			// conversion
			long double dNumber1 = std::stold(sNumber1);
			long double dNumber2 = std::stold(sNumber2);

			long double dFinalNumAns = 0;

			dFinalNumAns = std::powl(dNumber1, dNumber2);

			// g. Replace string with power of numbers
			sCalculationString.replace(nNumber1StartLocation, (nNumber2EndLocation + 1) - nNumber1StartLocation, FormatValueForAlgorithm(dFinalNumAns));

			if (bDisplayWorkingOutProcess) std::cout << "--> " << FormatStringForDisplay(sCalculationString) << '\n';

			// Check for infinity- if found, return with error level 4
			if (sCalculationString.find("inf", 0) != std::string::npos) {
				nErrorLevel = 4;
				return dFinalResult;
			}
		}

		// 2. Division and Multiplication
		while (true)
		{
			// 0 for multiplication, 1 for division
			bool bMultiplicationOrDivision = 0;
			size_t nSymbolLocation = 0;

			// Check for any unnecessary arithmetic symbols before the run
			sCalculationString = RemoveUnnecessaryArithmeticSymbols(sCalculationString);

			// Find what's first - division or multiplication
			if (sCalculationString.find("/", 0) > sCalculationString.find("*", 0)) {
				// a. Find multiplication symbol (*) and get location; if not found, break
				nSymbolLocation = sCalculationString.find("*", 0);
				if (nSymbolLocation == std::string::npos) break;

				bMultiplicationOrDivision = 0;
			}
			else {
				// a. Find division symbol (/) and get location; if not found, break
				nSymbolLocation = sCalculationString.find("/", 0);
				if (nSymbolLocation == std::string::npos) break;

				bMultiplicationOrDivision = 1;
			}

			// b. Check if both sides of the division/multiplication symbol are numbers and the symbol isn't at the start/end of string
			//    - if all true, continue. If not, post syntax error and exit
			if (nSymbolLocation > 0 && nSymbolLocation < sCalculationString.length() - 1) {
				if (std::isdigit(sCalculationString[nSymbolLocation - 1]) == false) {
					nErrorLevel = 2;
					dFinalResult = 0;
					return dFinalResult;
				}
				if (std::isdigit(sCalculationString[nSymbolLocation + 1]) == false && sCalculationString[nSymbolLocation + 1] != '-') {
					nErrorLevel = 2;
					dFinalResult = 0;
					return dFinalResult;
				}
				else if (sCalculationString[nSymbolLocation + 1] == '-') {
					if (nSymbolLocation + 1 < sCalculationString.length()) {
						if (std::isdigit(sCalculationString[nSymbolLocation + 2]) == false) {
							nErrorLevel = 2;
							dFinalResult = 0;
							return dFinalResult;
						}
					}
					else {
						nErrorLevel = 2;
						dFinalResult = 0;
						return dFinalResult;
					}
				}
			}
			else {
				nErrorLevel = 2;
				dFinalResult = 0;
				return dFinalResult;
			}

			// c. Check for first number location before division/multiplication sign
			size_t nNumber1StartLocation = nSymbolLocation - 1;
			for (; nNumber1StartLocation >= 0; nNumber1StartLocation--) {
				if (isdigit(sCalculationString[nNumber1StartLocation]) == false && sCalculationString[nNumber1StartLocation] != cDefaultDecimalPoint) { // decimal point accepted
					nNumber1StartLocation++; // to avoid the non-numerical character
					break; // Start of number found
				}
				else if (nNumber1StartLocation <= 0) {
					break; // Start of number found
				}
			}

			// d. Check for second number location after division/multiplication sign
			size_t nNumber2EndLocation = nSymbolLocation + 1;
			for (; nNumber2EndLocation <= sCalculationString.length() - 1; nNumber2EndLocation++) {
				if (nNumber2EndLocation == nSymbolLocation + 1) {
					if (sCalculationString[nNumber2EndLocation] == '-') {
						// continue as this should be a negative number
						continue;
					}
				}
				if (isdigit(sCalculationString[nNumber2EndLocation]) == false && sCalculationString[nNumber2EndLocation] != cDefaultDecimalPoint) { // decimal point accepted
					nNumber2EndLocation--; // to avoid the non-numerical character
					break; // Start of number found
				}
				else if (nNumber2EndLocation >= sCalculationString.length() - 1) {
					break; // Start of number found
				}
			}

			// Check new locations if negative was included
			bool bNegativeIsOperator = false;
			if (nNumber1StartLocation > 0) {
				if (sCalculationString[nNumber1StartLocation - 1] == '-') {
					nNumber1StartLocation--; // Include negative

					// Possible negative operator
					if (nNumber1StartLocation > 0) {
						if (isdigit(sCalculationString[nNumber1StartLocation - 1])) {
							bNegativeIsOperator = true;
						}
					}
				}
			}


			// e. Convert strings into numbers with checks
			std::string sNumber1 = sCalculationString.substr(nNumber1StartLocation, nSymbolLocation - nNumber1StartLocation);
			std::string sNumber2 = sCalculationString.substr(nSymbolLocation + 1, nNumber2EndLocation - nSymbolLocation);

			if (isNumber(sNumber1) == false) {
				nErrorLevel = 3;
				dFinalResult = 0;
				return dFinalResult;
			}
			if (isNumber(sNumber2) == false) {
				nErrorLevel = 3;
				dFinalResult = 0;
				return dFinalResult;
			}

			// conversion
			long double dNumber1 = std::stold(sNumber1);
			long double dNumber2 = std::stold(sNumber2);

			long double dFinalNumAns = 0.0;

			if (bMultiplicationOrDivision == 0)
			{
				dFinalNumAns = dNumber1 * dNumber2;
			}
			else if (bMultiplicationOrDivision == 1)
			{
				// f. Divide by 0 check to prevent errors
				if (dNumber2 == 0.0) {
					// Divide by 0 math error
					nErrorLevel = 1;
					dFinalResult = 0;
					return dFinalResult;
				}
				else dFinalNumAns = dNumber1 / dNumber2;
			}

			// Check for negative operator and if the end number was positive
			if (dFinalNumAns >= 0 && bNegativeIsOperator == true) {
				// g. Divide numbers and replace whole division/multiplication operation string with new number and a plus at the beginning to prevent combination of 2 numbers without operator
				sCalculationString.replace(nNumber1StartLocation, (nNumber2EndLocation + 1) - nNumber1StartLocation, '+' + FormatValueForAlgorithm(dFinalNumAns));
			}
			else {
				// g. Divide numbers and replace whole division/multiplication operation string with new number
				sCalculationString.replace(nNumber1StartLocation, (nNumber2EndLocation + 1) - nNumber1StartLocation, FormatValueForAlgorithm(dFinalNumAns));
			}

			if (bDisplayWorkingOutProcess) std::cout << "--> " << FormatStringForDisplay(sCalculationString) << '\n';

			// Check for infinity- if found, return with error level 4
			if (sCalculationString.find("inf", 0) != std::string::npos) {
				nErrorLevel = 4;
				return dFinalResult;
			}
		}

		// 3. Addition and Subtraction
		size_t nSearchLocation = 0;
		while (true) {
			size_t nSymbolLocation = 0;

			// 1 is addition, 0 is subtraction
			bool bIsAdditionOrSubtraction = true;

			// Check for any unnecessary arithmetic symbols before the run
			sCalculationString = RemoveUnnecessaryArithmeticSymbols(sCalculationString);

			// a. Find what is first (addition or subtraction)
			if (sCalculationString.find("+", nSearchLocation) < sCalculationString.find("-", nSearchLocation))
			{
				bIsAdditionOrSubtraction = true;
				// Get location of addition symbol
				nSymbolLocation = sCalculationString.find("+", nSearchLocation);
				if (nSymbolLocation == std::string::npos) break;
			}
			else {
				bIsAdditionOrSubtraction = false;
				// Get location of subtraction symbol
				nSymbolLocation = sCalculationString.find("-", nSearchLocation);
				if (nSymbolLocation == std::string::npos) break;

				else if (nSymbolLocation == 0)
				{
					// Try to find another symbol again by changing the search location to search beyond the scope of this symbol
					nSearchLocation++;
					continue;
				}
			}

			// b. Check if both sides of the add/subtraction symbol are numbers and the symbol isn't at the start/end of string
			//    - if all true, continue. If not, post syntax error and exit
			if (nSymbolLocation > 0 && nSymbolLocation < sCalculationString.length() - 1) {
				if (std::isdigit(sCalculationString[nSymbolLocation - 1]) == false) {
					nErrorLevel = 2;
					dFinalResult = 0;
					return dFinalResult;
				}
				if (std::isdigit(sCalculationString[nSymbolLocation + 1]) == false) {
					nErrorLevel = 2;
					dFinalResult = 0;
					return dFinalResult;
				}
			}
			else {
				nErrorLevel = 2;
				dFinalResult = 0;
				return dFinalResult;
			}

			// c. Check for first number location before add/subtraction sign
			size_t nNumber1StartLocation = nSymbolLocation - 1;
			for (; nNumber1StartLocation >= 0; nNumber1StartLocation--) {
				if (isdigit(sCalculationString[nNumber1StartLocation]) == false && sCalculationString[nNumber1StartLocation] != cDefaultDecimalPoint) { // decimal point accepted
					nNumber1StartLocation++; // to avoid the non-numerical character
					break; // Start of number found
				}
				else if (nNumber1StartLocation <= 0) {
					break; // Start of number found
				}
			}

			// d. Check for second number location after add/subtraction sign
			size_t nNumber2EndLocation = nSymbolLocation + 1;
			for (; nNumber2EndLocation <= sCalculationString.length() - 1; nNumber2EndLocation++) {
				if (isdigit(sCalculationString[nNumber2EndLocation]) == false && sCalculationString[nNumber2EndLocation] != cDefaultDecimalPoint) { // decimal point accepted
					nNumber2EndLocation--; // to avoid the non-numerical character
					break; // Start of number found
				}
				else if (nNumber2EndLocation >= sCalculationString.length() - 1) {
					break; // Start of number found
				}
			}

			// Check if first number starts with a negative symbol; if so, include it
			if (nNumber1StartLocation > 0) {
				if (sCalculationString[nNumber1StartLocation - 1] == '-') {
					nNumber1StartLocation--;
				}
			}

			// e. Convert strings into numbers with checks
			std::string sNumber1 = sCalculationString.substr(nNumber1StartLocation, (nSymbolLocation)-nNumber1StartLocation);
			std::string sNumber2 = sCalculationString.substr(nSymbolLocation + 1, nNumber2EndLocation - (nSymbolLocation));

			if (isNumber(sNumber1) == false) {
				nErrorLevel = 3;
				dFinalResult = 0;
				return dFinalResult;
			}
			if (isNumber(sNumber2) == false) {
				nErrorLevel = 3;
				dFinalResult = 0;
				return dFinalResult;
			}

			// conversion
			long double dNumber1 = std::stold(sNumber1);
			long double dNumber2 = std::stold(sNumber2);

			long double dFinalAddSubAns = 0;

			// f. If symbol was addition, add numbers. If symbol was subtraction, subtract numbers.
			if (bIsAdditionOrSubtraction == 1) {
				dFinalAddSubAns = dNumber1 + dNumber2;
			}
			else {
				dFinalAddSubAns = dNumber1 - dNumber2;
			}

			// Replace whole part of calculation string with calculated product
			sCalculationString.replace(nNumber1StartLocation, (nNumber2EndLocation + 1) - nNumber1StartLocation, FormatValueForAlgorithm(dFinalAddSubAns));

			if (bDisplayWorkingOutProcess) std::cout << "--> " << FormatStringForDisplay(sCalculationString) << '\n';

			// Check for infinity- if found, return with error level 4
			if (sCalculationString.find("inf", 0) != std::string::npos) {
				nErrorLevel = 4;
				return dFinalResult;
			}

			// Set the search location to 0 - all negative checks are no longer needed and are finished
			nSearchLocation = 0;
		}

		// Check for any unnecessary arithmetic symbols after all runs
		sCalculationString = RemoveUnnecessaryArithmeticSymbols(sCalculationString);

		// Check for infinity- if found, return with error level 4
		if (sCalculationString.find("inf", 0) != std::string::npos) {
			nErrorLevel = 4;
			return dFinalResult;
		}

		// 4. Convert and return final answer. Check validity of number; if not number, report syntax error
		if (isNumber(sCalculationString)) {
			dFinalResult = std::stold(sCalculationString);
			if (dFinalResult == -0) dFinalResult = 0;

			// All went well as it seems
			nErrorLevel = 0;
		}
		else {
			nErrorLevel = 2;
			dFinalResult = 0;
		}

		return dFinalResult;
	}

	// EradicateBrackets - Removes brackets from calculation string and converts them to constant numbers.
	//                     It has support for numbers before and after the brackets that are attached to the brackets.
	// Arguments: sCalculationString - The calculation string to remove and simplify the brackets from.
	// Return values: Modified string
	//
	std::string EradicateBrackets(std::string sCalculationString) {

		// Loop the check until nothing else found
		for (size_t nFirstBracketPos = 0, nLastBracketPos = 0; nFirstBracketPos != std::string::npos && nLastBracketPos != std::string::npos;)
		{
			// 1. Find '(' and get location
			nFirstBracketPos = sCalculationString.find_last_of('(');

			// No first bracket found - return updated string
			if (nFirstBracketPos == std::string::npos) break;

			// 2. Find ')' and get location
			nLastBracketPos = sCalculationString.find(')', nFirstBracketPos);

			// No final bracket found - calculate up to end of string
			if (nLastBracketPos == std::string::npos) {
				nErrorLevel = 2;
				return "";
			}

			// 3. Get substring of locations, pass into calculation function, get result
			std::string sBracketCalculation = sCalculationString.substr(nFirstBracketPos + 1, (nLastBracketPos - 1) - (nFirstBracketPos));

			// Display working out if wanted by user and switch
			if (bDisplayWorkingOutProcess) {
				std::cout << "Bracket (" << FormatStringForDisplay(sBracketCalculation) << "):\n";
			}

			long double dBracketResult = 0;

			dBracketResult = Calculate(sBracketCalculation);

			//
			// Constants Check
			//
			if (sBracketCalculation == "pi") {
				dBracketResult = dPiConstant;
				if (bDisplayWorkingOutProcess) std::cout << "Pi constant in this bracket, so immediately substituting for that...\n";
			}
			else if (sBracketCalculation == "e") {
				dBracketResult = dEulerConstant;
				if (bDisplayWorkingOutProcess) std::cout << "Euler constant in this bracket, so immediately substituting for that...\n";
			}
			else if (sBracketCalculation == "ans") {
				dBracketResult = dLastAns;
				if (bDisplayWorkingOutProcess) std::cout << "'Ans' user-space variable in this bracket, so immediately substituting for that...\n";
			}

			// Formulate string (done in case the user puts bracket next to/after literal number)
			std::string sFinalBracketProduct = "";
			bool bSpecialFormatting = false;

			//
			// Trigonometric Functions and Square Roots
			//
			if (sCalculationString.rfind("asin(", nFirstBracketPos) + 4 == nFirstBracketPos && sCalculationString.rfind("asin(", nFirstBracketPos) != std::string::npos) {

				// acos, asin or atan cannot accept anything above 1 or below -1 due to how sine waves work
				if (dBracketResult < -1.0 || dBracketResult > 1.0) {
					nErrorLevel = 1;
					return "";
				}

				dBracketResult = std::asinl(dBracketResult);
				dBracketResult *= dRadToDeg; // Convert final answer from radians to degrees
				nFirstBracketPos -= 4;
				if (bDisplayWorkingOutProcess) std::cout << "asin() function calculation result: " << FormatValueForDisplay(dBracketResult) << "\n";
			}
			else if (sCalculationString.rfind("acos(", nFirstBracketPos) + 4 == nFirstBracketPos && sCalculationString.rfind("acos(", nFirstBracketPos) != std::string::npos) {

				// acos, asin cannot accept anything above 1 or below -1 due to how sine waves work
				if (dBracketResult < -1.0 || dBracketResult > 1.0) {
					nErrorLevel = 1;
					return "";
				}

				dBracketResult = std::acosl(dBracketResult);
				dBracketResult *= dRadToDeg;
				nFirstBracketPos -= 4;
				if (bDisplayWorkingOutProcess) std::cout << "acos() function calculation result: " << FormatValueForDisplay(dBracketResult) << "\n";
			}
			else if (sCalculationString.rfind("atan(", nFirstBracketPos) + 4 == nFirstBracketPos && sCalculationString.rfind("atan(", nFirstBracketPos) != std::string::npos) {
				dBracketResult = std::atanl(dBracketResult);
				dBracketResult *= dRadToDeg;
				nFirstBracketPos -= 4;
				if (bDisplayWorkingOutProcess) std::cout << "atan() function calculation result: " << FormatValueForDisplay(dBracketResult) << "\n";
			}
			else if (sCalculationString.rfind("sin(", nFirstBracketPos) + 3 == nFirstBracketPos && sCalculationString.rfind("sin(", nFirstBracketPos) != std::string::npos) {
				dBracketResult = std::sinl(dBracketResult * dDegToRad); // Formula for converting degrees to radians: Deg * (Pi / 180) (std::sinl only accepts radians)
				nFirstBracketPos -= 3;
				if (bDisplayWorkingOutProcess) std::cout << "sin() function calculation result: " << FormatValueForDisplay(dBracketResult) << "\n";
			}
			else if (sCalculationString.rfind("cos(", nFirstBracketPos) + 3 == nFirstBracketPos && sCalculationString.rfind("cos(", nFirstBracketPos) != std::string::npos) {
				dBracketResult = std::cosl(dBracketResult * dDegToRad);
				nFirstBracketPos -= 3;
				if (bDisplayWorkingOutProcess) std::cout << "cos() function calculation result: " << FormatValueForDisplay(dBracketResult) << "\n";
			}
			else if (sCalculationString.rfind("tan(", nFirstBracketPos) + 3 == nFirstBracketPos && sCalculationString.rfind("tan(", nFirstBracketPos) != std::string::npos) {
				dBracketResult = std::tanl(dBracketResult * dDegToRad);
				nFirstBracketPos -= 3;
				if (bDisplayWorkingOutProcess) std::cout << "tan() function calculation result: " << FormatValueForDisplay(dBracketResult) << "\n";
			}
			else if (sCalculationString.rfind("sqrt(", nFirstBracketPos) + 4 == nFirstBracketPos && sCalculationString.rfind("sqrt(", nFirstBracketPos) != std::string::npos) {
				if (bDisplayWorkingOutProcess) std::cout << "Square Root of " << dBracketResult << " calculation result: ";
				dBracketResult = std::sqrtl(dBracketResult);
				nFirstBracketPos -= 4;
				if (bDisplayWorkingOutProcess) std::cout << FormatValueForDisplay(dBracketResult) << "\n";
			}
			else if (sCalculationString.rfind("cbrt(", nFirstBracketPos) + 4 == nFirstBracketPos && sCalculationString.rfind("cbrt(", nFirstBracketPos) != std::string::npos) {
				if (bDisplayWorkingOutProcess) std::cout << "Cube Root of " << dBracketResult << " calculation result: ";
				dBracketResult = std::cbrtl(dBracketResult);
				nFirstBracketPos -= 4;
				if (bDisplayWorkingOutProcess) std::cout << FormatValueForDisplay(dBracketResult) << "\n";
			}

			if (nLastBracketPos < sCalculationString.length() - 2) 
			{
				// The only solution for the power of a bracketed negative without conflicting the Calculate() function
				// is to do the calculation on the bracket discovery. It may be a non-elegant solution
				// as it's basically duplicating code from the Calculate() function, but it's the most
				// elegant solution that I could think of, as trying to unpack the bracket from the 
				// Calculate() function would defeat the entire purpose of this function and it would also
				// lead to the introduction of many complex variables that just make the code unreadable.
				//
				if (sCalculationString[nLastBracketPos + 1] == '^' && dBracketResult < 0)
				{
					// Check for second number location after power sign
					size_t nSymbolLocation = nLastBracketPos + 1;
					size_t nNumber2EndLocation = nSymbolLocation + 1;

					// Post message that power calculation is being done
					if (bDisplayWorkingOutProcess) std::cout << "Power symbol found after bracket. Doing power calculation...\n";

					for (; nNumber2EndLocation <= sCalculationString.length() - 1; nNumber2EndLocation++) {
						if (nNumber2EndLocation == nSymbolLocation + 1) {
							if (sCalculationString[nNumber2EndLocation] == '-') {
								// continue as this should be a negative number
								continue;
							}
						}
						if (isdigit(sCalculationString[nNumber2EndLocation]) == false && sCalculationString[nNumber2EndLocation] != cDefaultDecimalPoint) { // decimal point accepted
							nNumber2EndLocation--; // to avoid the non-numerical character
							break; // Start of number found
						}
						else if (nNumber2EndLocation >= sCalculationString.length() - 1) {
							break; // Start of number found
						}
					}

					// Get number as string and check number validity
					std::string sNumber2 = sCalculationString.substr(nSymbolLocation + 1, nNumber2EndLocation - (nSymbolLocation));
					if (sNumber2 == "") {
						nErrorLevel = 2;
						return "";
					}
					if (isNumber(sNumber2) == false) {
						nErrorLevel = 3;
						return "";
					}

					// Convert to number after validity check
					long double dNumber2 = std::stold(sNumber2);

					// Output final answer if working out is enabled
					if (bDisplayWorkingOutProcess) std::cout << "Power of " << dBracketResult << " calculation answer: ";

					// Calculate
					dBracketResult = std::powl(dBracketResult, dNumber2);
					
					if (bDisplayWorkingOutProcess) std::cout << FormatValueForDisplay(dBracketResult) << ". This answer will be substituted.\n";

					// Set the final bracket to the end number location to set that for replacement too
					nLastBracketPos = nNumber2EndLocation;
				}
			}

			// Don't output a newline on constant/trigonometric/square&cube root calculation discovery
			if (bDisplayWorkingOutProcess) {
				std::cout << "\n";
			}

			// Check for any required multiplying of brackets before the first bracket
			if (nFirstBracketPos > 0) {
				if (std::isdigit(sCalculationString[nFirstBracketPos - 1]) || sCalculationString[nFirstBracketPos - 1] == ')') 
				{
					sFinalBracketProduct = "*" + FormatValueForAlgorithm(dBracketResult);
					bSpecialFormatting = true;
				}
			}

			// Check for any required multiplying of brackets after the last bracket
			if (nLastBracketPos < sCalculationString.length() - 1) {
				if (std::isdigit(sCalculationString[nLastBracketPos + 1]) || sCalculationString[nLastBracketPos + 1] == '(') 
				{
					sFinalBracketProduct = FormatValueForAlgorithm(dBracketResult) + "*";
					bSpecialFormatting = true;
				}
			}

			if (!bSpecialFormatting) 
			{
				sFinalBracketProduct = FormatValueForAlgorithm(dBracketResult);
			}

			// 4. Change the bracket part of calculation string into result
			// NOTE: nLastBracketPos is incremented by 1 before being used, as the parameter is for the number of characters to replace, so it's based on 1 and not 0
			sCalculationString.replace(nFirstBracketPos, (nLastBracketPos + 1) - nFirstBracketPos, sFinalBracketProduct);
		}

		return sCalculationString;
	}

	// RemoveUnnecessaryArithmeticSymbols - Simplifies/removes arithmetic operators that are placed next to each other,
	//                                 e.g) 5+-++--+-5 simplifies to 5+5
	// Arguments: sCalculationString - The calculation string to remove the arithmetic symbols from.
	// Return values: Modified string
	//
	std::string RemoveUnnecessaryArithmeticSymbols(std::string sCalculationString) 
	{
		/*
			We will need to look for the following strings and consolidate repeatedly,
			until there is nothing left:
				1. +-
				2. -+
				3. --
				4. ++
			The first two get consolidated to a minus symbol, and the last two get consolidated
			to a plus symbol.
		*/

		// Loop check until all processes finished and nothing else found
		bool bFirstNpos = false, bSecondNpos = false, bThirdNpos = false, bFourthNpos = false;
		for (size_t nPlusMinusLocation = 0; !bFirstNpos || !bSecondNpos || !bThirdNpos || !bFourthNpos;)
		{

			// 1. Search for +-
			nPlusMinusLocation = sCalculationString.find("+-", 0);
			if (nPlusMinusLocation != std::string::npos) {
				sCalculationString.replace(nPlusMinusLocation, 2, "-");
				bFirstNpos = false;
			}
			else bFirstNpos = true;

			// 2. Search for -+
			nPlusMinusLocation = sCalculationString.find("-+", 0);
			if (nPlusMinusLocation != std::string::npos) {
				sCalculationString.replace(nPlusMinusLocation, 2, "-");
				bSecondNpos = false;
			}
			else bSecondNpos = true;

			// 3. Search for --
			nPlusMinusLocation = sCalculationString.find("--", 0);
			if (nPlusMinusLocation != std::string::npos) {
				sCalculationString.replace(nPlusMinusLocation, 2, "+");
				bThirdNpos = false;
			}
			else bThirdNpos = true;

			// 4. Search for ++
			nPlusMinusLocation = sCalculationString.find("++", 0);
			if (nPlusMinusLocation != std::string::npos) {
				sCalculationString.replace(nPlusMinusLocation, 2, "+");
				bFourthNpos = false;
			}
			else bFourthNpos = true;
		}

		// Check for a plus symbol at the beginning of calculation string; if there is one, remove it- it would cause errors
		if (sCalculationString[0] == '+') {
			sCalculationString.erase(0, 1);
		}

		// Return
		return sCalculationString;
	}

	// RemoveAllFormatting - Remove spaces/thousands separators from mathematical expression.
	//                     - Required in case user uses spaces/thousands separators, as spaces/thousands separators will cause unintended syntax errors.
	// Arguments: sCalculationString - The calculation string to remove space/thousands separator characters from.
	// Return values: Modified string
	//
	std::string RemoveAllFormatting(std::string sCalculationString) 
	{

		// Remove spaces
		while (true) {
			size_t nSpaceLocation = sCalculationString.find(' ', 0);

			// No spaces found; exit
			if (nSpaceLocation == std::string::npos) break;

			// Erase/remove the space from the string
			sCalculationString.erase(nSpaceLocation, 1);
		}

		// Remove thousands separator
		while (true) {
			size_t nSeparatorLocation = sCalculationString.find(cThousandsSeparator, 0);

			// No thousands separator found; exit
			if (nSeparatorLocation == std::string::npos) break;

			// Erase/remove the thousands separator from the string
			sCalculationString.erase(nSeparatorLocation, 1);
		}

		// Change decimal places to default
		while (true) {
			// Change the last decimal point to default notation
			size_t nDecimalPointPos = sCalculationString.find_last_of(cEuropeanDecimalPoint);

			// Don't do anything when not found
			if (nDecimalPointPos == std::string::npos) {
				break;
			}

			// Erase/remove the decimal point from the string
			sCalculationString.replace(nDecimalPointPos, 1, std::string(1, cDefaultDecimalPoint));
		}

		return sCalculationString;
	}

public:

	// A switch for displaying calculator working out process.
	bool bDisplayWorkingOutProcess = false;

	// CalculationAlgorithm Constructor
	CalculationAlgorithm() {
		nErrorLevel = 0;
		bDisplayWorkingOutProcess = false;

		// Set notation
		UseEuropeanNotation(false);

		VerbosityDisplay("CalculationAlgorithm Object Created.\n");

		return;
	}

	// CalculationAlgorithm Destructor
	~CalculationAlgorithm() {
		VerbosityDisplay("CalculationAlgorithm Object Destroyed.\n");

		return;
	}

	// GetLastCalculationErrorInfo - Gets the last calculation error and outputs details as a string.
	// Arguments: None
	// Return values: Error information string.
	//
	std::string GetLastCalculationErrorInfo() 
	{
		// Switch for error information
		switch (nErrorLevel) {
		case 0:
			return "No error has occured.";
			break;
		case 1:
			return "A fatal math error has occured. Dividing by 0 or asin/acos argument is not between -1 and 1?";
			break;
		case 2:
			return "A fatal syntax error has occured. Unknown syntax or missing open/close brackets?";
			break;
		case 3:
			return "A fatal unknown number parsing error has occured.";
			break;
		case 4:
			return "The final answer is either infinity or too large to calculate.";
			break;
		default:
			return "An unknown error occured when getting error information.";
			break;
		}

		return "";
	}

	// GetLastCalculationErrorValue - Gets the last calculation error value. The value return meanings are as follows:
	//              0) No error has occured.
	//              1) A math error has occured (e.g division by 0).
	//              2) A syntax error has occured.
	//              3) An unknown number parsing error has occured.
	// Arguments: None
	// Return values: Last calculation error value.
	//
	inline short int GetLastCalculationErrorValue() 
	{
		return nErrorLevel;
	}

	// SafeCalculate - Calculates and finds the product of a calculation string provided by the user,
	//                 which utilises all features contained in the calculation algorithm to prepare
	//                 the calculation string for the calculation.
	// Arguments: sCalculationString - The string to find the product of/calculate.
	// Return values: The calculated answer in long double-grade precision.
	// 
	// NOTE: This function returns 0.0 if any errors occur, and sets the error level accordingly.
	//
	long double SafeCalculate(std::string sCalculationString) {

		// Reset all errors
		nErrorLevel = 0;

		// Firstly, make all parts of string lowercase letters (calculator is not case-sensitive)
		for (size_t i = 0; i < sCalculationString.length(); i++) {
			// Convert character to lowercase
			sCalculationString[i] = std::tolower(sCalculationString[i]);
		}

		// Secondly, remove all user formatting to make calculation string compatible with all algorithm operations
		sCalculationString = RemoveAllFormatting(sCalculationString);

		// Check for any errors
		if (nErrorLevel > 0) return 0.0;

		// Thirdly, eradicate all brackets from the calculation string
		sCalculationString = EradicateBrackets(sCalculationString);

		// Check for any errors
		if (nErrorLevel > 0) return 0.0;

		// Display indication if working out process enabled
		if (bDisplayWorkingOutProcess) std::cout << "Removing unnecessary arithmetic symbols...\n";

		// Fourthly, check for any extra plus/minus pairs and convert them to their necessary +/-
		sCalculationString = RemoveUnnecessaryArithmeticSymbols(sCalculationString);

		// Display final result if working out process output enabled
		if (bDisplayWorkingOutProcess) std::cout << "New calculation: " << FormatStringForDisplay(sCalculationString) << "\n\n";

		// Check for any errors
		if (nErrorLevel > 0) return 0.0;

		// Finally, calculate string
		long double dFinalResult = Calculate(sCalculationString);

		// Check for any errors
		if (nErrorLevel > 0) return 0.0;
		else dLastAns = dFinalResult; // For (Ans) user-space variable

		return dFinalResult;
	}

	// UseEuropeanNotation - When the toggle value is TRUE, allow the use of European Number Notation, with '.' being set as the thousands separator, and ',' being the decimal point.
	//                     - When set to FALSE though, the notation resets to default, with ',' being set as the thousands separator, and '.' being the decimal point.
	// Arguments - bToggleValue: The value for setting the notaton. Values are explained in the description above.
	// Return values: TRUE or 1 for success, FALSE or 0 for fail. If FALSE/0 is returned, a bad value has been passed.
	//
	bool UseEuropeanNotation(bool bToggleValue) {
		switch (bToggleValue) {
		case true:
			cDecimalPoint = cEuropeanDecimalPoint;
			cThousandsSeparator = cEuropeanThousandsSeparator;
			break;
		case false:
			cDecimalPoint = cDefaultDecimalPoint;
			cThousandsSeparator = cDefaultThousandsSeparator;
			break;
		default:
			VerbosityDisplay("In CalculationAlgorithm::UseEuropeanNotation(): ERROR - Unknown value recieved from bToggleValue, therefore setting notation failed. Not true/false (1/0).\n");
			return false;
		}

		return true;
	}
};