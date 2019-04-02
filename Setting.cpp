#include "Setting.h"

Setting * Setting::m_Instance = nullptr;

Setting * Setting::getInstance(int argc, char** argv)
{
	if (!m_Instance)
	{
		m_Instance = new Setting(argc, argv);
	}

	return m_Instance;
}

Setting::Setting(int argc, char** argv)
{
	uint8 index;

	if ((4 <= argc) && (argc <= 5))
	{
		try {
			/**mode of encoding*/
			/**reading path:*/
			std::string path = argv[1];
			Utils::replaceAll(path, '\\', '/');
			setPath(path);

			try
			{
				/**requesting type of preconditioner*/
				index = std::stoi(std::string(argv[2]));

				if ((index >= 0) && (index <= 10))
				{
					m_GeneralArguments.emplace_back(static_cast<uint8>(index));
				}
				else
				{
					throw std::invalid_argument("Preconditioner " + std::to_string(index) + " not implemented yet\n");
				}

			}
			catch (const std::invalid_argument & e)
			{
				std::cerr << e.what();
				return;
			}

			try
			{
				/**requesting type of compressor*/
				index = std::stoi(std::string(argv[3]));

				if (index < 0 || index > 1)
				{
					throw std::invalid_argument("Compressor " + std::to_string(index) + " not implemented yet\n");
				}
				m_GeneralArguments.emplace_back(static_cast<uint8>(index));
			}
			catch (const std::invalid_argument & e)
			{
				std::cerr << e.what();
				return;
			}

			if (argc == 5)
			{
				std::string x = std::string(argv[4]);

				if (x.compare("-v") == 0)
				{
					/**check parameter for execute verbosing*/
					setDoVerbose(x.compare("-v") == 0);
				}
				else
				{
					throw std::invalid_argument("Value " + std::to_string(index) + " not allowed for execute fast up or verbosing!\n");
				}
			}

			m_CryptionType = CryptionType::ENCODE;
			setIsSettingOk(true);
		}
		catch (const std::invalid_argument & ex)
		{
			std::cerr << ex.what();
			return;
		}
	}
	else if ((argc == 2) || (argc == 3))
	{
		try
		{
			/**reading path:*/
			std::string path = argv[1];
			Utils::replaceAll(path, '\\', '/');
			setPath(path);

			if (!Utils::checkResource(path))
			{
				throw std::invalid_argument("path " + path + " points not to a binary file generated by this algorithm\n");
			}

			/**mode of decoding*/

			if (argc == 3)
			{
				try {
					/**check parameter for execute verbosing*/
					std::string x = std::string(argv[2]);

					setDoVerbose(x.compare("-v") == 0);
				}
				catch (const std::invalid_argument & ex)
				{
					std::cerr << ex.what();
					return;
				}
			}

			m_CryptionType = CryptionType::DECODE;
			setIsSettingOk(true);
		}
		catch (const std::invalid_argument & ex)
		{
			std::cerr << ex.what();
			return;
		}
	}

	if (getDoVerbose() && m_IsSettingOk)
	{
		MeasureTime::getInstance()->start();
	}

}

