﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using PalmSens.Comm;
using PalmSens.Devices;
using PalmSens.Core.Simplified.Data;
using System.Threading.Tasks;
using PalmSens.Core.Simplified.InternalStorage;

namespace PalmSens.Core.Simplified.WinForms
{
    public partial class PSCommSimpleWinForms: Component, IPlatform, IDisposable
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="PSCommSimpleWinForms"/> class.初始化类的新实例
        /// This class handles the communication with a device and it is used
        /// to perform measurements and control the device manually.
        /// 该类处理与设备的通信，并被使用
        /// 进行测量和手动控制设备。
        /// </summary>
        /// <param name="container">The containing windows form or control.包含窗口的窗体或控件。</param>
        public PSCommSimpleWinForms(IContainer container) : this()
        {
            container.Add(this);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PSCommSimpleWinForms"/> class.
        /// This class handles the communication with a device and it is used
        /// to perform measurements and control the device manually.
        /// </summary>
        public PSCommSimpleWinForms()
        {
            InitializeComponent();
            PalmSens.Windows.CoreDependencies.Init(); //Initiates PSSDK threading dependencies//初始化PSSDK线程依赖
            InitAsyncFunctionality(Environment.ProcessorCount); //Initiate the asynchronous functions in the SDK//初始化SDK中的异步函数
            _psCommSimple = new PSCommSimple(this);
        }

        #region Properties
        /// <summary>
        /// Instance of the platform independent PSCommSimple class that manages measurements and manual control
        /// 平台独立的PSCommSimple类的实例，用于管理测量和手动控制
        /// </summary>
        private PSCommSimple _psCommSimple;

        /// <summary>
        /// The parent control required to invoke events to the UI thread which is done to prevent threading exceptions
        /// 向UI线程调用事件所需的父控件，这样做是为了防止线程异常
        /// </summary>
        private Control _parent = null;

        /// <summary>
        /// Gets or sets the parent form.获取或设置父窗体。
        /// </summary>
        /// <value>
        /// The parent form.
        /// </value>
        /// <exception cref="System.NullReferenceException">Parent form/control cannot be null.</exception>
        public Control Parent
        {
            get
            {
                return _parent;
            }
            set
            {
                if (value == null)
                    throw new NullReferenceException("Parent form/control cannot be null.");
                _parent = value as Control;
            }
        }

        /// <summary>
        /// The device handler class which handles the connection to the device设备处理程序类，用于处理到设备的连接
        /// </summary>
        private DeviceHandler _deviceHandler = new DeviceHandler();

        /// <summary>
        /// Gets the CommManager for the current connection.获取当前连接的CommManager。
        /// </summary>
        /// <value>
        /// The CommManager, null when there is no active connection to a device.///CommManager，当没有主动连接到设备时为空。
        /// </value>
        public CommManager Comm => _psCommSimple.Comm;

        /// <summary>
        /// Gets a value indicating whether <see cref="PSCommSimple"/> is connected to a device.获取一个值，该值指示11是否连接到设备。
        /// </summary>
        /// <value>
        ///   <c>true</c> if connected; otherwise, <c>false</c>.已连接:true
        /// </value>
        public bool Connected => _psCommSimple.Connected;

        /// <summary>
        /// Gets the connected device type.获取连接的设备类型。
        /// </summary>
        /// <value>
        /// The connected device type.
        /// </value>
        public enumDeviceType ConnectedDevice => _psCommSimple.ConnectedDevice;

        /// <summary>
        /// Returns an array of connected devices.返回连接设备的数组。
        /// </summary>
        /// <value>
        /// The connected devices.
        /// </value>
        public Device[] ConnectedDevices => _deviceHandler.ConnectedDevices;

        /// <summary>
        /// Returns an array of connected devices.返回连接设备的数组。
        /// </summary>
        /// <value>
        /// The connected devices.
        /// </value>
        [Obsolete("Please use GetConnectedDevicesAsync method instead")]
        public Task<Device[]> ConnectedDevicesAsync() => _deviceHandler.GetConnectedDevicesAsync();

        /// <summary>
        /// Gets the state of the connected device.获取连接设备的状态。
        /// </summary>
        /// <value>
        /// The state of the device.
        /// </value>
        public CommManager.DeviceState DeviceState => _psCommSimple.DeviceState;

        /// <summary>
        /// Gets a value indicating whether [cell is on].获取一个值，该值指示[单元格是否打开]。
        /// </summary>
        /// <value>
        ///   <c>true</c> if [cell is on]; otherwise, <c>false</c>.
        /// </value>
        public bool CellOn => _psCommSimple.IsCellOn;

        /// <summary>
        /// Gets the capabilities of the connected device.获取连接设备的功能。
        /// </summary>
        /// <value>
        /// The device capabilities.
        /// </value>
        public DeviceCapabilities Capabilities => _psCommSimple.Capabilities;

        /// <summary>
        /// Gets or sets a value indicating whether to enable devices connected via bluetooth.
        /// 获取或设置一个值，该值指示是否启用通过蓝牙连接的设备。
        /// </summary>
        /// <value>
        ///   <c>true</c> Enable scan for devices over bluetooth; Disable scan for devices over bluetooth <c>false</c>.
        ///   启用蓝牙扫描设备;禁用蓝牙设备扫描
        /// </value>
        public bool EnableBluetooth
        {
            get => _deviceHandler.EnableBluetooth;
            set => _deviceHandler.EnableBluetooth = value;
        }

        /// <summary>
        /// Gets or sets a value indicating whether to enable devices in VCP mode.
        /// 获取或设置一个值，该值指示是否在VCP模式下启用设备。
        /// </summary>
        /// <value>
        ///   <c>true</c> Enable scan for VCP devices on serial port; Disable scan for VCP devices on serial port <c>false</c>.
        /// </value>
        public bool EnableSerialPort
        {
            get => _deviceHandler.EnableSerialPort;
            set => _deviceHandler.EnableSerialPort = value;
        }

        /// <summary>
        /// Determines whether [the specified method] is compatible with the device.
        /// 判断[指定的方法]是否与设备兼容。
        /// </summary>
        /// <param name="method">The method.</param>
        /// <returns>
        ///   <c>true</c> if the method is valid; otherwise, <c>false</c>.
        /// </returns>
        public bool IsValidMethod(Method method) => _psCommSimple.IsValidMethod(method);
        #endregion

        #region Functions
        /// <summary>
        /// Required initialization for using the async functionalities of the PalmSens SDK.
        /// The amount of simultaneous operations will be limited to prevent performance issues.
        /// When possible it will leave one core free for the UI.
        /// </summary>
        /// <param name="nCores">The number of CPU cores.</param>
        public void InitAsyncFunctionality(int nCores)
        {
            SynchronizationContextRemover.Init(nCores > 1 ? nCores - 1 : 1);
        }

        /// <summary>
        /// Returns an array of connected devices.
        /// </summary>
        public async Task<Device[]> GetConnectedDevicesAsync() => await _deviceHandler.GetConnectedDevicesAsync();

        /// <summary>
        /// Connects to the device with the highest priority.
        /// </summary>
        public void Connect()
        {
            _psCommSimple.Comm = _deviceHandler.Connect();
        }

        /// <summary>
        /// Connects to the device with the highest priority.
        /// </summary>
        public async Task ConnectAsync()
        {
            _psCommSimple.Comm = await _deviceHandler.ConnectAsync();
        }

        /// <summary>
        /// Connects to the specified device.
        /// </summary>
        /// <param name="device">The device.</param>
        public void Connect(Device device)
        {
            _psCommSimple.Comm = _deviceHandler.Connect(device);
        }

        /// <summary>
        /// Connects to the specified device.连接到指定的设备。
        /// </summary>
        /// <param name="device">The device.</param>
        public async Task ConnectAsync(Device device)
        {
            _psCommSimple.Comm = (await _deviceHandler.ConnectAsync(device)).Comm;
        }

        /// <summary>
        /// Disconnects from the connected device.
        /// </summary>
        public void Disconnect()
        {
            _psCommSimple.Disconnect();
        }

        /// <summary>
        /// Disconnects from the connected device.
        /// </summary>
        public Task DisconnectAsync() => _psCommSimple.DisconnectAsync();

        /// <summary>
        /// Turns the cell on.
        /// </summary>
        public void TurnCellOn()
        {
            _psCommSimple.TurnCellOn();            
        }

        /// <summary>
        /// Turns the cell on.
        /// </summary>
        public Task TurnCellOnAsync() => _psCommSimple.TurnCellOnAsync();

        /// <summary>
        /// Turns the cell off.
        /// </summary>
        public void TurnCellOff()
        {
            _psCommSimple.TurnCellOff();
        }

        /// <summary>
        /// Turns the cell off.
        /// </summary>
        public Task TurnCellOffAsync() => _psCommSimple.TurnCellOffAsync();

        /// <summary>
        /// Sets the cell potential.
        /// </summary>
        /// <param name="potential">The potential.</param>
        public void SetCellPotential(float potential)
        {
            _psCommSimple.SetCellPotential(potential);
        }

        /// <summary>
        /// Sets the cell potential.
        /// </summary>
        /// <param name="potential">The potential.</param>
        public Task SetCellPotentialAsync(float potential) => _psCommSimple.SetCellPotentialAsync(potential);

        /// <summary>
        /// Reads the cell potential.
        /// </summary>
        /// <returns>The potential (V).</returns>
        public float ReadCellPotential()
        {
            return _psCommSimple.ReadCellPotential();
        }

        /// <summary>
        /// Reads the cell potential.
        /// </summary>
        /// <returns>The potential (V).</returns>
        public Task<float> ReadCellPotentialAsync() => _psCommSimple.ReadCellPotentialAsync();

        /// <summary>
        /// Sets the cell current.
        /// </summary>
        /// <param name="current">The current.</param>
        public void SetCellCurrent(float current)
        {
            _psCommSimple.SetCellCurrent(current);
        }

        /// <summary>
        /// Sets the cell current.
        /// </summary>
        /// <param name="current">The current.</param>
        public Task SetCellCurrentAsync(float current) => _psCommSimple.SetCellCurrentAsync(current);

        /// <summary>
        /// Reads the cell current.
        /// </summary>
        /// <returns>The current (µA).</returns>
        public float ReadCellCurrent()
        {
            return _psCommSimple.ReadCellCurrent();
        }

        /// <summary>
        /// Reads the cell current.
        /// </summary>
        /// <returns>The current (µA).</returns>
        public Task<float> ReadCellCurrentAsync() => _psCommSimple.ReadCellCurrentAsync();

        /// <summary>
        /// Sets the current range.
        /// </summary>
        /// <param name="currentRange">The current range.</param>
        public void SetCurrentRange(CurrentRange currentRange)
        {
            _psCommSimple.SetCurrentRange(currentRange);
        }

        /// <summary>
        /// Sets the current range.
        /// </summary>
        /// <param name="currentRange">The current range.</param>
        public Task SetCurrentRangeAsync(CurrentRange currentRange) => _psCommSimple.SetCurrentRangeAsync(currentRange);

        /// <summary>
        /// Runs a measurement as specified in the method on the connected device.
        /// </summary>
        /// <param name="method">The method containing the measurement parameters.</param>
        /// <param name="muxChannel">The mux channel to measure on.</param>
        /// <returns>
        /// A SimpleMeasurement instance containing all the data related to the measurement.
        /// </returns>
        /// <exception cref="System.NullReferenceException">Not connected to a device.</exception>
        /// <exception cref="System.ArgumentException">Method is incompatible with the connected device.</exception>
        /// <exception cref="System.Exception">Could not start measurement.</exception>
        public SimpleMeasurement Measure(Method method, int muxChannel)
        {
            return _psCommSimple.Measure(method, muxChannel);
        }

        /// <summary>
        /// Runs a measurement as specified in the method on the connected device.
        /// </summary>
        /// <param name="method">The method containing the measurement parameters.</param>
        /// <param name="muxChannel">The mux channel to measure on.</param>
        /// <param name="taskBarrier">The task barrier, optional parameter used to synchronise the start of a measurement between channels.</param>
        /// <returns>
        /// A SimpleMeasurement instance containing all the data related to the measurement.
        /// </returns>
        /// <exception cref="System.NullReferenceException">Not connected to a device.</exception>
        /// <exception cref="System.ArgumentException">Method is incompatible with the connected device.</exception>
        /// <exception cref="System.Exception">Could not start measurement.</exception>
        public Task<SimpleMeasurement> MeasureAsync(Method method, int muxChannel, TaskBarrier taskBarrier = null) => _psCommSimple.MeasureAsync(method, muxChannel, taskBarrier);

        /// <summary>
        /// Runs a measurement as specified in the method on the connected device.
        /// </summary>
        /// <param name="method">The method containing the measurement parameters.</param>
        /// <returns>A SimpleMeasurement instance containing all the data related to the measurement.</returns>
        public SimpleMeasurement Measure(Method method)
        {
            return _psCommSimple.Measure(method);
        }

        /// <summary>
        /// Runs a measurement as specified in the method on the connected device.
        /// </summary>
        /// <param name="method">The method containing the measurement parameters.</param>
        /// <param name="taskBarrier">The task barrier, optional parameter used to synchronise the start of a measurement between channels.</param>
        /// <returns>A SimpleMeasurement instance containing all the data related to the measurement.</returns>
        public Task<SimpleMeasurement> MeasureAsync(Method method, TaskBarrier taskBarrier = null) => _psCommSimple.MeasureAsync(method, taskBarrier);

        /// <summary>
        /// Aborts the active measurement.
        /// </summary>
        public void AbortMeasurement()
        {
            _psCommSimple.AbortMeasurement();
        }

        /// <summary>
        /// Aborts the current active measurement.
        /// </summary>
        public Task AbortMeasurementAsync() => _psCommSimple.AbortMeasurementAsync();

        /// <summary>
        /// Runs a MethodSCRIPT on the device, ignoring any output returned by the script.
        /// </summary>
        /// <param name="script">The MethodSCRIPT.</param>
        /// <param name="timeout">The timeout.</param>
        /// <exception cref="NullReferenceException">Not connected to a device</exception>
        /// <exception cref="Exception">Device must be in idle mode to run a MethodSCRIPT</exception>
        public void StartSetterMethodScript(string script, int timeout = 500)
        {
            _psCommSimple.StartGetterMethodScript(script, timeout);
        }

        /// <summary>
        /// Runs a MethodSCRIPT on the device, ignoring any output returned by the script.
        /// </summary>
        /// <param name="script">The MethodSCRIPT.</param>
        /// <param name="timeout">The timeout.</param>
        /// <exception cref="NullReferenceException">Not connected to a device</exception>
        /// <exception cref="Exception">Device must be in idle mode to run a MethodSCRIPT</exception>
        public Task StartSetterMethodScriptAsync(string script, int timeout = 500) =>
            _psCommSimple.StartSetterMethodScriptAsync(script, timeout);

        /// <summary>
        /// Runs a MethodSCRIPT on the device and returns any output returned by the script.
        /// </summary>
        /// <param name="script">The MethodSCRIPT.</param>
        /// <param name="timeout">The timeout.</param>
        /// <returns></returns>
        public string StartGetterMethodScript(string script, int timeout = 2500)
        {
            return _psCommSimple.StartGetterMethodScript(script, timeout);
        }

        /// <summary>
        /// Runs a MethodSCRIPT on the device and returns any output returned by the script.
        /// </summary>
        /// <param name="script">The MethodSCRIPT.</param>
        /// <param name="timeout">The timeout.</param>
        /// <returns></returns>
        public Task<string> StartGetterMethodScriptAsync(string script, int timeout = 2500) =>
            _psCommSimple.StartGetterMethodScriptAsync(script, timeout);

        /// <summary>
        /// Reads the specified digital line(s) state(s).
        /// Which lines to read from are specified in a bitmask.
        /// Bit 0 is for GPIO0, bit 1 for GPIO1, etc. Bits that are high correspond with a high output signal
        /// </summary>
        /// <param name="bitMask">A bitmask specifying which digital lines to read (0 = ignore, 1 = read).</param>
        /// <returns>Bitmask that represents the specified lines output signal (0 = low, 1 = high).</returns>
        public uint ReadDigitalLine(byte bitMask)
        {
            return _psCommSimple.ReadDigitalLine(bitMask);
        }

        /// <summary>
        /// Reads the specified digital line(s) state(s).
        /// Which lines to read from are specified in a bitmask.
        /// Bit 0 is for GPIO0, bit 1 for GPIO1, etc. Bits that are high correspond with a high output signal
        /// </summary>
        /// <param name="bitMask">A bitmask specifying which digital lines to read (0 = ignore, 1 = read).</param>
        /// <returns>Bitmask that represents the specified lines output signal (0 = low, 1 = high).</returns>
        public Task<uint> ReadDigitalLineAsync(byte bitMask) => _psCommSimple.ReadDigitalLineAsync(bitMask);

        /// <summary>
        /// Sets the digital lines output signal high or low.
        /// The output signal for the digital lines are defined in a bitmask.
        /// Bit 0 is for GPIO0, bit 1 for GPIO1, etc. Bits that are high correspond with a high output signal
        /// </summary>
        /// <param name="bitMask">A bitmask specifying the output of the digital lines (0 = low, 1 = high).</param>
        public void SetDigitalOutput(int bitMask)
        {
            _psCommSimple.SetDigitalOutput(bitMask);
        }

        /// <summary>
        /// Sets the digital lines output signal high or low.
        /// The output signal for the digital lines are defined in a bitmask.
        /// Bit 0 is for GPIO0, bit 1 for GPIO1, etc. Bits that are high correspond with a high output signal
        /// </summary>
        /// <param name="bitMask">A bitmask specifying the output of the digital lines (0 = low, 1 = high).</param>
        public Task SetDigitalOutputAsync(int bitMask) => _psCommSimple.SetDigitalOutputAsync(bitMask);

        /// <summary>
        /// Sets the specified digital lines to input/output and set the output signal of the lines set to output
        /// The output signal for the digital lines are defined in a bitmask.
        /// Bit 0 is for GPIO0, bit 1 for GPIO1, etc. Bits that are high correspond with a high output signal
        /// </summary>
        /// <param name="bitMask">A bitmask specifying the output signal of the digital lines (0 = low, 1 = high).</param>
        /// <param name="configGPIO">A bitmask specifying the the mode of digital lines (0 = input, 1 = output).</param>
        public void SetDigitalOutput(int bitMask, int configGPIO)
        {
            _psCommSimple.SetDigitalOutput(bitMask, configGPIO);
        }

        /// <summary>
        /// Sets the specified digital lines to input/output and set the output signal of the lines set to output
        /// The output signal for the digital lines are defined in a bitmask.
        /// Bit 0 is for GPIO0, bit 1 for GPIO1, etc. Bits that are high correspond with a high output signal
        /// </summary>
        /// <param name="bitMask">A bitmask specifying the output signal of the digital lines (0 = low, 1 = high).</param>
        /// <param name="configGPIO">A bitmask specifying the the mode of digital lines (0 = input, 1 = output).</param>
        public Task SetDigitalOutputAsync(int bitMask, int configGPIO) =>
            _psCommSimple.SetDigitalOutputAsync(bitMask, configGPIO);

        /// <summary>
        /// Validates whether the specified method is compatible with the capabilities of the connected device.
        /// </summary>
        /// <param name="method">The method containing the measurement parameters.</param>
        /// <param name="isValidMethod">if set to <c>true</c> [is valid method].</param>
        /// <param name="errors">The errors.</param>
        /// <exception cref="System.NullReferenceException">Not connected to a device.</exception>
        /// <exception cref="System.ArgumentNullException">The specified method cannot be null.</exception>
        public void ValidateMethod(Method method, out bool isValidMethod, out List<string> errors)
        {
            _psCommSimple.ValidateMethod(method, out isValidMethod, out errors);
        }
        
        /// <summary>
        /// Get an internal storage handler that will read the current connected device stored files. This is only for devices that have internal storage.
        /// </summary>
        /// <exception cref="InvalidOperationException">This exception is thrown when the device is not connected or if the device does not support storage.</exception>
        /// <returns>A new instance of the internal storage handler for the current connection.</returns>
        public IInternalStorageBrowser GetInternalStorageBrowser()
        {
            return _psCommSimple.GetInternalStorageBrowser();
        }

        #endregion

        #region Platform interface
        /// <summary>
        /// Invokes event to UI thread if required.
        /// </summary>
        /// <param name="method">The method.</param>
        /// <param name="args">The arguments.</param>
        /// <returns></returns>
        /// <exception cref="System.NullReferenceException">Parent control not set.</exception>
        public bool InvokeIfRequired(Delegate method, params object[] args)
        {
            if (Parent == null)
                throw new NullReferenceException("Parent control not set.");
            if (Parent.InvokeRequired) //Check if event needs to be cast to the UI thread
            {
                Parent.BeginInvoke(method, args); //Recast event to UI thread
                return true;
            }
            return false;
        }

        /// <summary>
        /// Disconnects from device with the specified CommManager. 
        /// Warning use the platform independent method Disconnect() instead.
        /// Otherwise the generic PSCommSimple does not unsubscribe from the CommManager correctly
        /// which may result in it not being released from the memory.
        /// </summary>
        /// <param name="comm">The comm.</param>
        public void Disconnect(CommManager comm)
        {
            _deviceHandler.Disconnect(comm);
        }

        /// <summary>
        /// Disconnects from device with the specified CommManager. 
        /// Warning use the platform independent method Disconnect() instead.
        /// Otherwise the generic PSCommSimple does not unsubscribe from the CommManager correctly
        /// which may result in it not being released from the memory.
        /// </summary>
        /// <param name="comm">The comm.</param>
        public Task DisconnectAsync(CommManager comm) => _deviceHandler.DisconnectAsync(comm);
        #endregion

        #region events
        /// <summary>
        /// Occurs when a device status package is received, these packages are not sent during a measurement.
        /// </summary>
        public event StatusEventHandler ReceiveStatus
        {
            add { _psCommSimple.ReceiveStatus += value; }
            remove { _psCommSimple.ReceiveStatus -= value; }
        }

        /// <summary>
        /// Occurs at the start of a new measurement.
        /// </summary>
        public event EventHandler MeasurementStarted
        {
            add { _psCommSimple.MeasurementStarted += value; }
            remove { _psCommSimple.MeasurementStarted -= value; }
        }

        /// <summary>
        /// Occurs when a measurement has ended.
        /// </summary>
        public event EventHandler<Exception> MeasurementEnded
        {
            add { _psCommSimple.MeasurementEnded += value; }
            remove { _psCommSimple.MeasurementEnded -= value; }
        }

        /// <summary>
        /// Occurs when a new [SimpleCurve starts receiving data].
        /// </summary>
        public event PSCommSimple.SimpleCurveStartReceivingDataHandler SimpleCurveStartReceivingData
        {
            add { _psCommSimple.SimpleCurveStartReceivingData += value; }
            remove { _psCommSimple.SimpleCurveStartReceivingData -= value; }
        }

        /// <summary>
        /// Occurs when the devive's [state changed].
        /// </summary>
        public event CommManager.StatusChangedEventHandler StateChanged
        {
            add { _psCommSimple.StateChanged += value; }
            remove { _psCommSimple.StateChanged -= value; }
        }

        /// <summary>
        /// Occurs when a device is [disconnected].
        /// </summary>
        public event DisconnectedEventHandler Disconnected
        {
            add { _psCommSimple.Disconnected += value; }
            remove { _psCommSimple.Disconnected -= value; }
        }
        #endregion

        /// <summary>
        /// Releases all resources used by the <see cref="T:System.ComponentModel.Component" />.
        /// </summary>
        public new void Dispose()
        {
            _psCommSimple.Dispose();
            base.Dispose();
        }
    }

}
