#pragma once

#ifndef SETTING_H__
#define SETTING_H__

#include <cogra/types.h>
#include <string>

#include "ANSTypes.h"
#include "CryptionType.h"
#include "MeasureTime.h"
#include "Utils.h"

/**the class Setting:
*
*holds the values the User passed by as command line arguments
*
*/
class Setting
{
private:

	static Setting * m_Instance;

	std::string m_Path;
	bool m_IsSettingOk = false, m_DoVerbose=false;
	std::vector<uint32> m_GeneralArguments;
	CryptionType m_CryptionType;


	/*
	*Constructor: set command line arguments
	*/
	Setting(int argc, char** argv);

public:

	Setting() = default;

	/**yields an instance of this class*/
	static Setting * getInstance(int argc, char** argv);

	//set & get follows below here

	void setPath(const std::string& path) { m_Path = path; }
	std::string getPath(void) const { return m_Path; }

	void setDoVerbose(const bool doVerbose) { m_DoVerbose = doVerbose; }
	bool getDoVerbose(void) { return m_DoVerbose; }

	void setIsSettingOk(const bool isSettingOk) { m_IsSettingOk = isSettingOk; }
	bool getIsSettingOk(void) { return m_IsSettingOk; }

	std::vector<uint32> getGeneralArguments(void) const { return m_GeneralArguments; }

	CryptionType getTypeOfCryption(void) const { return m_CryptionType; }
};

#endif