#ifndef SLAVE_H
#define SLAVE_H

#include "sockets.h"

#ifdef DEBUG
    #define SLAVE_DEBUG
    // #undef SLAVE_DEBUG
#endif

namespace Sorokin {

//-----------------------------------------------------------------------------------------------------------------------------

    /**
     * @brief The Slave class - implementation of Slave-socket manager
     */
    class Slave {

    public:
        /// @brief Return error types
        enum err {
            undefinedError = -1,
            noError = 0
        };

    public:
        /// @defgroup Slave_setup_methods
        /// @{
        /// @brief Default constructor
        Slave() noexcept : _slaveSocket(nullptr) {}
        /// @brief accessSocket() - <Socket> object accessor
        virtual Socket*& accessSocket()& noexcept(false) {
            return _slaveSocket;
        }
        /// @}

        /// @defgroup Slave_usage_methods
        /// @{
        /**
         * @brief sendString - writes string to the socket according to the info given in the <Socket> object
         * @param[in] strToSend - string to send
         * @return 0 if no error occured, -1 else
         */
        virtual err sendString(const std::string& strToSend) noexcept(false);
        /**
         * @brief recvString - reads string from the socket according to the info given in the <Socket> object
         * @param[out] strToRecv - string to write recieved message
         * @return 0 if no error occured, -1 else
         */
        virtual err recvString(std::string& strToRecv) noexcept(false);
        /// @}

        /// @defgroup Slave_deleting_methods
        /// @{
        /**
         * @brief closeConnection - closes connection on socket
         * @return 0 in no error occured, -1 else
         */
        virtual err closeConnection() noexcept;
        /**
         * @brief deleteSocket - deletes <Socket> object
         * @throw std::exception if unable to delete Socket
         */
        virtual void deleteSocket() noexcept(false);
        /// @brief Default destructor
        virtual ~Slave() = default;
        /// @}

    protected:
        Socket* _slaveSocket;

    protected:
        /// @defgroup Slave_additional_methods
        /// @{
        /**
         * @brief printCError - prints <errno> msg
         * @param preErrorMsg - msg which will be printed before <errno> msg
         */
        void __printCError(std::string preErrorMsg) noexcept(false);
        /// @}

    private:
        /// @defgroup Slave_deleted_methods
        /// @{
        /// @brief Deleted copy constructor
        Slave(const Slave&) = delete;
        /// @brief Deleted move constructor
        Slave(Slave&&) = delete;
        /// @brief Deleted copy assigner
        Slave& operator =(const Slave&) = delete;
        /// @brief Deleted move assigner
        Slave& operator =(Slave&&) = delete;
        /// @}

    };

//-----------------------------------------------------------------------------------------------------------------------------

}

#endif // Slave_H
