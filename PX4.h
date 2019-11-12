/************************************************************/
/*    NAME: David Battle                                    */
/*    ORGN: Mission Systems Pty Ltd                         */
/*    (based on ArduSubComms by Muthukumaran Chandrasekaran)*/
/*    FILE: PX4.cpp                                         */
/*    DATE: 14 Dec 2018                                     */
/************************************************************/

#ifndef PX4_HEADER
#define PX4_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class PX4 : public CMOOSApp
{
	 public:
	   PX4();
	   ~PX4();

	 protected: // Standard MOOSApp functions to overload
	   bool OnNewMail(MOOSMSG_LIST &NewMail);
	   bool Iterate();
	   bool OnConnectToServer();
	   bool OnStartUp();

	 protected:
	   void RegisterVariables();

	 private: // State variables
	   bool                                        m_udp_mode;
	   unsigned char*                              m_buf;
	   std::string                                 m_udp_host;
	   std::string                                 m_udp_port;
	   std::string                                 m_serial_name;
	   unsigned int                                m_serial_baud;
	   boost::asio::io_service                     m_io;
	   boost::shared_ptr<boost::asio::serial_port> m_serial;
	   boost::shared_ptr<udp::socket>              m_udp;
	   udp::endpoint                               m_sender_endpoint;
};

#endif
