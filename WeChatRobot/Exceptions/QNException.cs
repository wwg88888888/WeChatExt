using System.Runtime.Serialization;
using System.Text;

namespace WeChat.Exceptions
{
    public class QNException : System.Exception
    {
        /// <summary>
        /// The error code
        /// </summary>
        public uint ErrorCode = 10000;
        /// <summary>
        /// The state
        /// </summary>
        public object State;

        /// <summary>
        /// Initializes a new instance of the <see cref="QNException" /> class.
        /// </summary>
        public QNException()
        {
            this.ErrorCode = 1;
            this.State = null;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="QNException" /> class.
        /// </summary>
        /// <param name="message">The message.</param>
        public QNException(string message)
            : base(message)
        {
            this.ErrorCode = 1;
            this.State = null;
        }


        public QNException(uint errorCode, string message) : base(message)

        {
            this.ErrorCode = errorCode;
            this.State = null;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="QNException" /> class.
        /// </summary>
        /// <param name="info">The info.</param>
        /// <param name="context">The context.</param>
        protected QNException(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
            this.ErrorCode = 1;
            this.State = null;
            this.ErrorCode = info.GetUInt32("ErrorCode");
            this.DetailMessage = info.GetString("DetailMessage");
            this.State = info.GetValue("State", typeof(object));
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="QNException" /> class.
        /// </summary>
        /// <param name="message">The message.</param>
        /// <param name="innerException">The inner exception.</param>
        public QNException(string message, System.Exception innerException)
            : base(message, innerException)
        {
            this.ErrorCode = 1;
            this.State = null;
        }


        /// <summary>
        /// Initializes a new instance of the <see cref="QNException" /> class.
        /// </summary>
        /// <param name="message">The message.</param>
        /// <param name="innerException">The inner exception.</param>
        public QNException(uint errorCode, string message, System.Exception innerException)

            : base(message, innerException)
        {
            this.ErrorCode = 1;
            this.State = null;
        }

        /// <summary>
        /// When overridden in a derived class, sets the <see cref="T:System.Runtime.Serialization.SerializationInfo" /> with information about the exception.
        /// </summary>
        /// <param name="info">The <see cref="T:System.Runtime.Serialization.SerializationInfo" /> that holds the serialized object data about the exception being thrown.</param>
        /// <param name="context">The <see cref="T:System.Runtime.Serialization.StreamingContext" /> that contains contextual information about the source or destination.</param>
        /// <PermissionSet>
        /// <IPermission class="System.Security.Permissions.FileIOPermission, mscorlib, Version=2.0.3600.0, Culture=neutral, PublicKeyToken=b77a5c561934e089" version="1" Read="*AllFiles*" PathDiscovery="*AllFiles*" />
        /// <IPermission class="System.Security.Permissions.SecurityPermission, mscorlib, Version=2.0.3600.0, Culture=neutral, PublicKeyToken=b77a5c561934e089" version="1" Flags="SerializationFormatter" />
        /// </PermissionSet>
        public override void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue("ErrorCode", this.ErrorCode);
            info.AddValue("DetailMessage", this.DetailMessage);
            info.AddValue("State", this.State);
            base.GetObjectData(info, context);
        }

        /// <summary>
        /// Returns a <see cref="T:System.String" /> that represents this instance.
        /// </summary>
        /// <returns>A <see cref="T:System.String" /> that represents this instance.</returns>
        /// <PermissionSet>
        /// <IPermission class="System.Security.Permissions.FileIOPermission, mscorlib, Version=2.0.3600.0, Culture=neutral, PublicKeyToken=b77a5c561934e089" version="1" PathDiscovery="*AllFiles*" />
        /// </PermissionSet>
        public override string ToString()
        {
            StringBuilder builder = new StringBuilder();
            builder.AppendLine(string.Format("ErrorCode: 0x{0:X8}", this.ErrorCode));
            builder.AppendLine("Message: " + this.Message);
            builder.AppendLine("Detail: " + ((this.DetailMessage == null) ? string.Empty : this.DetailMessage));
            builder.AppendLine(base.ToString());
            return builder.ToString();
        }

        /// <summary>
        /// Gets or sets the detail message.
        /// </summary>
        /// <value>The detail message.</value>
        public string DetailMessage { get; set; }
    }
}
