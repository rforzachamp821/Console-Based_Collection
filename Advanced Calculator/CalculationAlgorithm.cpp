
// CalculationAlgorithm - A calculator class with support for up to a theoretical 18 trillion characters per calculation line.
//
// It supports the modern implementation of the BIDMAS order of operations, negative numbers, spaces and is crash-safe.
// It also supports multipliers after brackets (without the multiplication symbol also works), powers, and support for multiple
// addition-subtraction operators beside each other. It also supports multiple brackets inside of each other.
// 
// The allowed operators are: (), *, /, +, -, and ^ (power of). You must have a number on each side of the operators.
// 
// Example allowed expressions are:
// -> 5+5(5)
// -> 7*4+(6/2)
// -> 5+++-+---+-6*7/5(3)4
// -> 5^2+9/3
// -> (10+5^2)((5*-2)+9-3^3)/2
//
// - This calculation algorithm is currently limited by the maximum precision of long double (16 digits of precision). 
// 
// - Please beware that the number outputted may not actually be 16 digits and possibly less, due to natural C++ limits.
// 
// - There is no support for logarithmic or trigonometric functions yet, nor is there any support for baked-in 
//   irrational numbers (pi, e, etc)
//
class CalculationAlgorithm {
protected:

	// Checks whether std::string argument is a string or not
	bool isNumber(const std::string sNumberTest) {
		// Not a number as there's nothing in the string
		if (sNumberTest.length() <= 0) return false;

		// Check if all digits are numbers
		for (size_t i = 0; i < sNumberTest.length(); i++) {
			if (i == 0) {
				// negative number
				if (sNumberTest[0] == '-') continue;
			}

			// skip character if it's a decimal point
			if (sNumberTest[i] == '.') continue;
			else if (isdigit(sNumberTest[i]) == false) return false;
		}

		// Test if above stold() range
		try {
			long double nRangeTest = std::stold(sNumberTest);
		}
		catch (const std::out_of_range& oorIsNumber) {
			std::cerr << "Exception caught - Number is too high/low (out of range).";
			return false;
		}

		return true;
	}

	// nErrorLevel - The type of error that occured when calculating. The following values are what they are:
	//              0) No error has occured.
	//              1) A math error has occured (e.g division by 0).
	//              2) A syntax error has occured.
	//              3) An unknown number parsing error has occured.
	// This can be recieved as a string using the CalculationAlgorithm::GetLastCalculationErrorInfo() function.
	//
	short int nErrorLevel = 0;

	// Calculate - Main calculation algorithm that calculates using each of the 4 basic mathematical operators,
	//             with negative number support along with that. The nErrorLevel variable is modified when 
	//             anything goes wrong.
	// Arguments: sCalculationString - The calculation string to work out the product of/find the answer of.
	// Return values: The calculated answer in long double-grade precision.
	//
	long double Calculate(std::string sCalculationString) {

		long double dFinalResult = 0;

		// Display first calculation if working out is allowed
		if (bDisplayWorkingOutProcess) std::cout << "--> " << sCalculationString << '\n';


		// 1. Powers
		while (true)
		{
			size_t nSymbolLocation = 0;

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
				if (isdigit(sCalculationString[nNumber1StartLocation]) == false && sCalculationString[nNumber1StartLocation] != '.') { // decimal point accepted
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
				if (isdigit(sCalculationString[nNumber2EndLocation]) == false && sCalculationString[nNumber2EndLocation] != '.') { // decimal point accepted
					nNumber2EndLocation--; // to avoid the non-numerical character
					break; // Start of number found
				}
				else if (nNumber2EndLocation >= sCalculationString.length() - 1) {
					break; // Start of number found
				}
			}

			// Check new locations if negative was included
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

			long double dFinalNumAns = 0;

			dFinalNumAns = std::powl(dNumber1, dNumber2);

			// g. Replace string with power of numbers
			sCalculationString.replace(nNumber1StartLocation, (nNumber2EndLocation + 1) - nNumber1StartLocation, std::to_string(dFinalNumAns));

			if (bDisplayWorkingOutProcess) std::cout << "--> " << sCalculationString << '\n';
		}

		// 2. Division and Multiplication
		while (true)
		{
			// 0 for multiplication, 1 for division
			bool bMultiplicationOrDivision = 0;
			size_t nSymbolLocation = 0;

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
				if (isdigit(sCalculationString[nNumber1StartLocation]) == false && sCalculationString[nNumber1StartLocation] != '.') { // decimal point accepted
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
				if (isdigit(sCalculationString[nNumber2EndLocation]) == false && sCalculationString[nNumber2EndLocation] != '.') { // decimal point accepted
					nNumber2EndLocation--; // to avoid the non-numerical character
					break; // Start of number found
				}
				else if (nNumber2EndLocation >= sCalculationString.length() - 1) {
					break; // Start of number found
				}
			}

			// Check new locations if negative was included
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

			long double dFinalNumAns = 0;

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

			// g. Divide numbers and replace whole division operation string with new number
			sCalculationString.replace(nNumber1StartLocation, (nNumber2EndLocation + 1) - nNumber1StartLocation, std::to_string(dFinalNumAns));

			if (bDisplayWorkingOutProcess) std::cout << "--> " << sCalculationString << '\n';
		}

		// 3. Addition and Subtraction
		size_t nSearchLocation = 0;
		while (true) {
			size_t nSymbolLocation = 0;

			// 1 is addition, 0 is subtraction
			bool bIsAdditionOrSubtraction = true;

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
				if (isdigit(sCalculationString[nNumber1StartLocation]) == false && sCalculationString[nNumber1StartLocation] != '.') { // decimal point accepted
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
				if (isdigit(sCalculationString[nNumber2EndLocation]) == false && sCalculationString[nNumber2EndLocation] != '.') { // decimal point accepted
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
			sCalculationString.replace(nNumber1StartLocation, (nNumber2EndLocation + 1) - nNumber1StartLocation, std::to_string(dFinalAddSubAns));

			if (bDisplayWorkingOutProcess) std::cout << "--> " << sCalculationString << '\n';

			// Set the search location to 0 - all negative checks are no longer needed and are finished
			nSearchLocation = 0;
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
			if (nLastBracketPos == std::string::npos) nLastBracketPos = sCalculationString.length() - 1;

			// 3. Get substring of locations, pass into calculation function, get result
			std::string sBracketCalculation = sCalculationString.substr(nFirstBracketPos + 1, (nLastBracketPos - 1) - (nFirstBracketPos));

			// Display working out if wanted by user and switch
			if (bDisplayWorkingOutProcess) {
				std::cout << "Bracket (" << sBracketCalculation << "):\n";

			}

			long double dBracketResult = Calculate(sBracketCalculation);

			if (bDisplayWorkingOutProcess) {
				std::cout << "\n";
			}

			// Formulate string (done in case the user puts bracket next to/after literal number)
			std::string sFinalBracketProduct = "";
			bool bSpecialFormatting = false;
			if (nFirstBracketPos > 0) {
				if (std::isdigit(sCalculationString[nFirstBracketPos - 1]) || sCalculationString[nFirstBracketPos - 1] == ')') {
					sFinalBracketProduct = "*" + std::to_string(dBracketResult);
					bSpecialFormatting = true;
				}
			}
			if (nLastBracketPos < sCalculationString.length() - 1) {
				if (std::isdigit(sCalculationString[nLastBracketPos + 1]) || sCalculationString[nLastBracketPos + 1] == '(') {
					sFinalBracketProduct = std::to_string(dBracketResult) + "*";
					bSpecialFormatting = true;
				}
			}

			if (!bSpecialFormatting) sFinalBracketProduct = std::to_string(dBracketResult);

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

		// Display indication if working out process enabled
		if (bDisplayWorkingOutProcess) std::cout << "Removing unnecessary arithmetic symbols...\n";

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

		// Display final result if working out process output enabled
		if (bDisplayWorkingOutProcess) std::cout << "New calculation: " << sCalculationString << "\n\n";

		// Return
		return sCalculationString;
	}

	// RemoveSpaces - Remove spaces from mathematical expression.
	//                Required in case user uses spaces, as spaces will cause unintended syntax errors.
	// Arguments: sCalculationString - The calculation string to remove space characters from.
	// Return values: Modified string
	//
	std::string RemoveSpaces(std::string sCalculationString) 
	{
		while (true) {
			size_t nSpaceLocation = sCalculationString.find(' ', 0);

			// No spaces found; exit
			if (nSpaceLocation == std::string::npos) break;

			// Erase/remove the space from the string
			sCalculationString.erase(nSpaceLocation, 1);
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

		return;
	}

	// CalculationAlgorithm Destructor
	~CalculationAlgorithm() {
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
			return "A fatal math error has occured.";
			break;
		case 2:
			return "A fatal syntax error has occured.";
			break;
		case 3:
			return "A fatal unknown number parsing error has occured.";
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
	// Arguments: sCalculationString - The string to find the procust of/calculate.
	// Return values: The calculated answer in long double-grade precision.
	//
	long double SafeCalculate(std::string sCalculationString) {

		// Firstly, remove all spaces to make calculation string compatible with everything else
		sCalculationString = RemoveSpaces(sCalculationString);

		// Secondly, eradicate all brackets from the calculation string
		sCalculationString = EradicateBrackets(sCalculationString);

		// Thirdly, check for any extra plus/minus pairs and convert them to their necessary +/-
		sCalculationString = RemoveUnnecessaryArithmeticSymbols(sCalculationString);

		// Finally, calculate string
		long double dFinalResult = Calculate(sCalculationString);

		return dFinalResult;
	}
};