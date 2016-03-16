#ifndef KPROTOCOL_DEF_H_
#define KPROTOCOL_DEF_H_

namespace YUN
{
	enum s2c_PROTOCOL
	{
		// next 0
		s2c_clientbegin = 0,
		s2c_login,
		s2c_chat,
		s2c_clientend,
	};

	enum c2s_PROTOCOL
	{
		// next 0
		c2s_serverbegin = 0,
		c2s_login,
		c2s_chat,
		c2s_serverend,
	};
}

#endif