﻿using System;
using System.Drawing;
using System.Windows.Forms;
using PalmSens;
using PalmSens.Comm;
using PalmSens.Core.Simplified.Data;
using PalmSens.Devices;
using PalmSens.Techniques;
using PalmSens.Techniques.Impedance;

namespace PSSDKPlotEISExample
{
    public partial class PSSDKPlotEISExample : Form
    {
        public PSSDKPlotEISExample()
        {
            InitializeComponent();

            InitCVMethod(); //Create the cyclic voltammetry method that defines the measurement parameters
            InitPlot(); //Resets and initiates the plot control
            DiscoverConnectedDevices(); //Populate the connected device combobox control填充连接的设备组合框控件
        }

        /// <summary>
        /// The instance of method class containing the Cyclic Voltammetry parameters
        /// ///包含循环伏安参数的方法类的实例
        /// </summary>
        private ImpedimetricMethod _methodEIS;

        /// <summary>
        /// The connected PalmSens & EmStat devices///连接的PalmSens & EmStat设备
        /// </summary>
        private Device[] _connectedDevices = new Device[0];

        /// <summary>
        /// The active SimpleMeasurement///激活的simplemmeasurement
        /// </summary>
        private SimpleMeasurement _activeMeasurement = null;

        /// <summary>
        /// Initializes the EIS method.///初始化EIS方法。
        /// </summary>
        private void InitCVMethod()
        {
            _methodEIS = new ImpedimetricMethod();
            _methodEIS.Potential = 0.0f; //0.0V DC potential
            _methodEIS.Eac = 0.01f; //0.01V RMS AC potential amplitude
            _methodEIS.ScanType = enumScanType.Fixed; //Single EIS scan
            _methodEIS.FreqType = enumFrequencyType.Scan; //EIS scan over a range of frequencies
            _methodEIS.MaxFrequency = 1e5f; //Max frequency is 100000Hz
            _methodEIS.MinFrequency = 10f; //Min frequency is 10Hz
            _methodEIS.nFrequencies = 11; //Sample at 11 different frequencies

            _methodEIS.EquilibrationTime = 1f; //Equilabrates the cell at the defined potential for 1 second before starting the measurement//测量开始前，使电池在定义的电位处平衡1秒
            _methodEIS.Ranging.StartCurrentRange = new CurrentRange(CurrentRanges.cr10mA); //Starts equilabration in the 10mA current range//在10mA电流范围内启动平衡
            _methodEIS.Ranging.MinimumCurrentRange = new CurrentRange(CurrentRanges.cr1uA); //Min current range 1µA
            _methodEIS.Ranging.MaximumCurrentRange = new CurrentRange(CurrentRanges.cr10mA); //Max current range 10mA
        }

        /// <summary>
        /// Initializes the plot control.///初始化控件。
        /// </summary>
        private void InitPlot()
        {
            //Nyquist plot
            plot.ClearAll(); //Clear all curves and data from plot//清除plot中的所有曲线和数据
            //Set the Axis labels//设置Axis标签
            plot.XAxisLabel = "Z Re";
            plot.YAxisLabel = "Z Im";
            plot.AddData("", new double[0], new double[0]); //Add a empty data array to draw an empty plot//添加一个空的数据数组来绘制一个空的图形

            //Bode plot
            plotBode.ClearAll(); //Clear all curves and data from plot
            //Set the Axis labels
            plotBode.XAxisLabel = "Frequency";
            plotBode.YAxisLabel = "Impedance";
            plotBode.YAxisSecondaryLabel = "Phase";
            plotBode.AddData("", new double[0], new double[0]); //Add a empty data array to draw an empty plot
            plotBode.XAxisType = SDKPlot.AxisType.Logarithmic;
            plotBode.YAxisType = SDKPlot.AxisType.Logarithmic;
        }

        /// <summary>
        /// Discovers the connected PalmSens & EmStat devices and adds them to the combobox control.
        /// 发现连接的PalmSens & EmStat设备，并将它们添加到组合框控件中。
        /// </summary>
        public void DiscoverConnectedDevices()
        {
            cmbDevices.Items.Clear();
            _connectedDevices = psCommSimpleWinForms.ConnectedDevices; //Discover connected devices/ /发现连接设备

            foreach (Device d in _connectedDevices) 
                cmbDevices.Items.Add(d.ToString()); //Add connected devices to control//添加连接的设备到控件

            int nDevices = cmbDevices.Items.Count;
            cmbDevices.SelectedIndex = nDevices > 0 ? 0 : -1;
            lbConsole.Items.Add($"Found {nDevices} device(s).");

            btnConnect.Enabled = nDevices > 0;
        }

        /// <summary>
        /// Handles the Click event of the btnRefresh control.///处理btnRefresh控件的Click事件。
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void btnRefresh_Click(object sender, EventArgs e)
        {
            DiscoverConnectedDevices(); //Add connected devices to the devices combobox control//添加连接的设备到设备组合框控件
        }

        /// <summary>
        /// Handles the Click event of the btnConnect control.///处理btnConnect控件的Click事件。
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (!psCommSimpleWinForms.Connected) //Determine whether a device is currently connected
            {
                if (cmbDevices.SelectedIndex == -1)
                    return;

                try
                {
                    //Connect to the device selected in the devices combobox control
                    psCommSimpleWinForms.Connect(_connectedDevices[cmbDevices.SelectedIndex]); 
                    lbConsole.Items.Add($"Connected to {psCommSimpleWinForms.ConnectedDevice.ToString()}");
                }
                catch (Exception ex)
                {
                    lbConsole.Items.Add(ex.Message);
                }
            }
            else
            {
                psCommSimpleWinForms.Disconnect(); //Disconnect from the connected device
            }

            //Update UI based on connection status//根据连接状态更新UI
            cmbDevices.Enabled = !psCommSimpleWinForms.Connected;
            btnRefresh.Enabled = !psCommSimpleWinForms.Connected;
            btnConnect.Text = psCommSimpleWinForms.Connected ? "Disconnect" : "Connect";
            btnMeasure.Enabled = psCommSimpleWinForms.Connected;
        }

        /// <summary>
        /// Handles the Click event of the btnMeasure control.处理btnMeasure控件的“单击”事件。
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.包含事件数据的实例。</param>
        private void btnMeasure_Click(object sender, EventArgs e)
        {
            if (psCommSimpleWinForms.DeviceState == PalmSens.Comm.CommManager.DeviceState.Idle) //Determine whether the device is currently idle or measuring
            {
                try
                {
                    plot.ClearAll(); //Clears data from previous measurements from the plot//清除绘图中以前的测量数据
                    plotBode.ClearAll();
                    _activeMeasurement = psCommSimpleWinForms.Measure(_methodEIS); //Start measurement defined in the method//开始方法中定义的测量

                    //Get simple curves for bode plot//得到简单的曲线为bode图
                    plotBode.AddSimpleCurve(_activeMeasurement.NewSimpleCurve(PalmSens.Data.DataArrayType.Frequency, PalmSens.Data.DataArrayType.Z, "Magnitude")[0]);
                    plotBode.AddSimpleCurve(_activeMeasurement.NewSimpleCurve(PalmSens.Data.DataArrayType.Frequency, PalmSens.Data.DataArrayType.Phase, "-Phase")[0], true); //Add phase curve to secondary axis//在次轴添加相位曲线
                }
                catch (Exception ex)
                {
                    lbConsole.Items.Add(ex.Message);
                }
            }
            else
            {
                try
                {
                    psCommSimpleWinForms.AbortMeasurement(); //Abort the active measurement
                }
                catch (Exception ex)
                {
                    lbConsole.Items.Add(ex.Message);
                }
            }
        }

        /// <summary>
        /// Raised when device status package is received (the device does not send status packages while measuring)
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="PalmSens.Comm.StatusEventArgs"/> instance containing the event data.</param>
        private void psCommSimpleWinForms_ReceiveStatus(object sender, PalmSens.Comm.StatusEventArgs e)
        {
            Status status = e.GetStatus(); //Get the PalmSens.Comm.Status instance from the event data
            double potential = status.PotentialReading.Value; //Get the potential
            double currentInRange = status.CurrentReading.ValueInRange; //Get the current expressed inthe active current range
            PalmSens.Comm.ReadingStatus currentStatus = status.CurrentReading.ReadingStatus; //Get the status of the current reading
            CurrentRange cr = status.CurrentReading.CurrentRange; //Get the active current range

            tbPotential.Text = potential.ToString("F3");
            tbCurrent.Text = currentInRange.ToString("F3");
            switch (currentStatus)
            {
                case PalmSens.Comm.ReadingStatus.OK:
                    tbCurrent.ForeColor = Color.Black;
                    break;
                case PalmSens.Comm.ReadingStatus.Overload:
                    tbCurrent.ForeColor = Color.Red;
                    break;
                case PalmSens.Comm.ReadingStatus.Underload:
                    tbCurrent.ForeColor = Color.Yellow;
                    break;
            }
            lblCurrentRange.Text = $"* {cr.ToString()}";
        }

        /// <summary>
        /// Raised when the connected device's status changes
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="CurrentState">State of the current.</param>
        private void psCommSimpleWinForms_StateChanged(object sender, PalmSens.Comm.CommManager.DeviceState CurrentState)
        {
            tbDeviceStatus.Text = CurrentState.ToString(); //Updates the device state indicator textbox
            btnConnect.Enabled = CurrentState == PalmSens.Comm.CommManager.DeviceState.Idle;
            btnMeasure.Text = CurrentState == PalmSens.Comm.CommManager.DeviceState.Idle ? "Measure" : "Abort";
        }

        /// <summary>
        /// Raised when the measurement is ended
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void psCommSimpleWinForms_MeasurementEnded(object sender, Exception e)
        {
            lbConsole.Items.Add("Measurement ended.");
        }

        /// <summary>
        /// Raised when the measurement is started当测量开始时引发
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void psCommSimpleWinForms_MeasurementStarted(object sender, EventArgs e)
        {
            lbConsole.Items.Add("Impedimetric measurement started.");
        }

        /// <summary>
        /// Raised when a Simple Curve in the active SimpleMeasurement starts receiving data
        /// 当活动simplemmeasurement中的简单曲线开始接收数据时引发
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="activeSimpleCurve">The active simple curve.</param>
        private void psCommSimpleWinForms_SimpleCurveStartReceivingData(object sender, SimpleCurve activeSimpleCurve)
        {
            //Only add default Nyquist curve to plot here, the bode plot curves have already been added above
            //这里只添加默认的Nyquist曲线来绘图，博德曲线已经在上面添加了
            if (activeSimpleCurve.Title != "Nyquist")
                return;
            
            plot.AddSimpleCurve(activeSimpleCurve);

            //Subscribe to the event indicating when the curve stops receiving new data points
            //订阅指示曲线何时停止接收新数据点的事件
            activeSimpleCurve.CurveFinished += activeSimpleCurve_CurveFinished;

            lbConsole.Items.Add("Curve is receiving new data...");
        }

        /// <summary>
        /// Raised when a SimpleCurve stops receiving new data points当SimpleCurve停止接收新数据点时引发
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="System.EventArgs" /> instance containing the event data.</param>
        private void activeSimpleCurve_CurveFinished(object sender, EventArgs e)
        {
            SimpleCurve activeCurve = sender as SimpleCurve;
            int nDataPointsReceived = activeCurve != null ? activeCurve.NDataPoints : 0;
            lbConsole.Items.Add($"一共接收 {nDataPointsReceived} 数据点");

            //Unsubscribe from the curves events to avoid memory leaks
            activeCurve.CurveFinished -= activeSimpleCurve_CurveFinished;
        }

        /// <summary>
        /// Raised when the instrument has been disconnected.
        /// If the instrument was disconnected due to a communication the exception is provided.
        /// In the case of a regular disconnect the exception will be set to null.
        /// 当仪器断开时发出。
        /// 如果仪器由于通信而断开，则提供异常。
        /// 在常规断开连接的情况下，异常将被设置为空。
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="exception">The exception.</param>
        private void psCommSimpleWinForms_Disconnected(object sender, Exception exception)
        {
            if (exception != null)
            {
                lbConsole.Items.Add(exception.Message);
            }

            lbConsole.Items.Add("断开连接");
            btnConnect.Text = "连接";
            btnConnect.Enabled = true;
            btnMeasure.Text = "测量";
        }
    }
}
