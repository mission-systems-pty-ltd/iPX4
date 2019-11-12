/************************************************************/
/*    NAME: David Battle                                    */
/*    ORGN: Mission Systems Pty Ltd                         */
/*    (based on ArduSubComms by Muthukumaran Chandrasekaran)*/
/*    FILE: PX4.cpp                                         */
/*    DATE: 14 Dec 2018                                     */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "mavlink.h"
#include "PX4.h"

#define BUFFER_LENGTH (60) // 279 is max mavlink2 message size

using namespace std;
using namespace boost::asio;

//---------------------------------------------------------
// Constructor

PX4::PX4()
{
  m_udp_mode    = true; // Default to SITL (via UDP)
  m_udp_host    = "127.0.0.1";
  m_udp_port    = "14540"; // PX4 SITL offboard control
  m_serial_name = "/dev/ttyUSB0";
  m_serial_baud = 115200;

  // Allocate Mavlink message buffer
  m_buf = new unsigned char [BUFFER_LENGTH];
}

//---------------------------------------------------------
// Destructor

PX4::~PX4()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PX4::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

    if (key == "MAVLINK_TRANSMIT") {

      size_t length = p->GetBinaryDataSize();
      memcpy(m_buf, p->GetBinaryData(), length);

      if (m_udp_mode) {

        // Send binary Mavlink message via UDP
        m_udp->send_to(buffer(m_buf, length), m_sender_endpoint);
      } else {

        // Send binary Mavlink message via serial
        write(*m_serial, buffer(m_buf, length));
      }
    }
  }

  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PX4::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PX4::Iterate()
{

  size_t Nbytes = 0;

  // Read Mavlink messages here in a loop
  // We'll need to iterate fairly quickly...
  if (m_udp_mode) {

    // Read from udp endpoint
    Nbytes = m_udp->receive_from(
        buffer(m_buf, BUFFER_LENGTH), m_sender_endpoint);
  } else {

    // Read from serial port
    Nbytes = read(*m_serial, buffer(m_buf, BUFFER_LENGTH));
  }

  // Publish received Mavlink message
  Notify("MAVLINK_RECEIVE", m_buf, Nbytes);

  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PX4::OnStartUp()
{
  SetAppFreq(0); // Fast as we can
  SetIterateMode(REGULAR_ITERATE_AND_COMMS_DRIVEN_MAIL);

  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);

  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {

    list<string>::iterator p;

    for(p=sParams.begin(); p!=sParams.end(); p++) {

      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      double dval  = atof(value.c_str());
      bool   bval  = (strcasecmp (value.c_str(), "TRUE") == 0 || dval != 0);

      if (param == "UDP_MODE") {
        cout << "UDP_MODE = " << bval << endl;
        m_udp_mode = bval;
      }
      else if (param == "UDP_HOST") {
        cout << "UDP_HOST = " << value << endl;
        m_udp_host = value;
      }
      else if (param == "UDP_PORT") {
        cout << "UDP_PORT = " << value << endl;
        m_udp_port = value;
      }
      else if (param == "SERIAL_NAME") {
        cout << "SERIAL_NAME = " << value << endl;
        m_serial_name = value;
      }
      else if (param == "SERIAL_BAUD") {
        cout << "SERIAL_BAUD = " << dval << endl;
        m_serial_baud = dval;
      }
    }
  }

  if (m_udp_mode) {

    // Setup a UDP socket with the specified endpoint
    m_udp = boost::shared_ptr<udp::socket>(
        new udp::socket(m_io, udp::endpoint(udp::v4(), atoi(m_udp_port.c_str()))));

  } else {

    // Setup a serial port with the given device name and baudrate
    m_serial = boost::shared_ptr<serial_port>(new serial_port(m_io, m_serial_name));
    m_serial->set_option(serial_port_base::baud_rate(m_serial_baud));
  }

  RegisterVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PX4::RegisterVariables()
{
  Register("MAVLINK_TRANSMIT", 0.0);
}
