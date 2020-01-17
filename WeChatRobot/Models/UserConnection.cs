using Fleck;

namespace WeChat
{
    public class UserConnection : User
    {
        public IWebSocketConnection Connection { get; set; }

        public int ProcessId { get; set; }
    }
}
