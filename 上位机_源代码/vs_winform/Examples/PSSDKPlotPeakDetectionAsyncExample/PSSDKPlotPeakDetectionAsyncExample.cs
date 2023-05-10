using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using PalmSens;
using PalmSens.Analysis;
using PalmSens.Comm;
using PalmSens.Core.Simplified.Data;
using PalmSens.Devices;
using PalmSens.Plottables;
using PalmSens.Techniques;
using PSSDKBasicAsyncExample;

namespace PSSDKPlotPeakDetectionAsyncExample
{
    public partial class PSSDKPlotPeakDetectionAsyncExample : Form
    {
        public PSSDKPlotPeakDetectionAsyncExample()
        {
            InitializeComponent();
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.EnableBluetooth = false;
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.EnableSerialPort = true;
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.Parent = this;
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.ReceiveStatus += new PalmSens.Comm.StatusEventHandler(this.psCommSimpleWinForms_ReceiveStatus);
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.MeasurementStarted += new System.EventHandler(this.psCommSimpleWinForms_MeasurementStarted);
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.MeasurementEnded += new System.EventHandler<System.Exception>(this.psCommSimpleWinForms_MeasurementEnded);
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.StateChanged += new PalmSens.Comm.CommManager.StatusChangedEventHandler(this.psCommSimpleWinForms_StateChanged);
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.Disconnected += new PalmSens.Core.Simplified.DisconnectedEventHandler(this.psCommSimpleWinForms_Disconnected);
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.SimpleCurveStartReceivingData += new PalmSens.Core.Simplified.PSCommSimple.SimpleCurveStartReceivingDataHandler(this.psCommSimpleWinForms_SimpleCurveStartReceivingData);
            InitCVMethod(); //Create the cyclic voltammetry method that defines the measurement parameters
            InitDataGrid();
            InitPlot(); //Resets and initiates the plot control
            DiscoverConnectedDevicesAsync(); //Populate the connected device combobox control
        }

        /// <summary>
        /// The instance of method class containing the Cyclic Voltammetry parameters
        /// </summary>
        private CyclicVoltammetry _methodCV;

        /// <summary>
        /// The connected PalmSens & EmStat devices
        /// </summary>
        private Device[] _connectedDevices = new Device[0];

        /// <summary>
        /// The active SimpleMeasurement
        /// </summary>
        private SimpleMeasurement _activeMeasurement = null;

        /// <summary>
        /// The minimum peak height in µA
        /// </summary>
        private double _minPeakHeight = 0.0001;

        /// <summary>
        /// Initializes the data grid view control.初始化数据网格视图控件。
        /// </summary>
        private void InitDataGrid()
        {
            dgvMeasurement.Rows.Clear();
            dgvMeasurement.Columns.Clear();

            DataGridViewTextBoxColumn dgvColID = new DataGridViewTextBoxColumn();
            dgvColID.HeaderText = "ID";
            dgvColID.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            dgvColID.ReadOnly = true;

            DataGridViewTextBoxColumn dgvColPotential = new DataGridViewTextBoxColumn();
            dgvColPotential.HeaderText = "Potential (V)";
            dgvColPotential.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            dgvColPotential.ReadOnly = true;

            DataGridViewTextBoxColumn dgvColCurrent = new DataGridViewTextBoxColumn();
            dgvColCurrent.HeaderText = "Current (µA)";
            dgvColCurrent.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            dgvColCurrent.ReadOnly = true;

            dgvMeasurement.Columns.Add(dgvColID);
            dgvMeasurement.Columns.Add(dgvColPotential);
            dgvMeasurement.Columns.Add(dgvColCurrent);
        }


        /// <summary>
        /// Initializes the CV method.
        /// </summary>
        private void InitCVMethod()
        {
            tbBegin.Text = Convert.ToString(beginV);
            tbHigh.Text = Convert.ToString(highV);
            tbLow.Text = Convert.ToString(lowV);
            tbStep.Text = Convert.ToString(stepV);
            tbRate.Text = Convert.ToString(rateV);
            tbEquilib.Text = Convert.ToString(equilibT);
            tbnScan.Text = Convert.ToString(nScan);
            tbFinal.Text = Convert.ToString(finalV);

            _methodCV = new CyclicVoltammetry(); //Create a new cyclic voltammetry method with the default settings
            _methodCV.BeginPotential = beginV; //Sets the potential to start the scan from设置启动扫描的位置
            _methodCV.Vtx1Potential = highV; //Sets the first potential where the scan direction reverses设置扫描方向反转的第一个电势
            _methodCV.Vtx2Potential = lowV; //Sets the second potential where the scan direction reverses设置扫描方向相反的第二个电势
            _methodCV.StepPotential = stepV; //Sets the step size设置步长
            _methodCV.Scanrate = rateV; //Sets the scan rate to 1 V/s/设置扫描速率为
            _methodCV.nScans = nScan; //Sets the number of scans设置扫描的次数

            _methodCV.EquilibrationTime = equilibT; //Equilabrates the cell at the defined potential for 5 seconds before starting the measurement在开始测量之前，将细胞在规定的电位处平衡5秒
            _methodCV.Ranging.StartCurrentRange = new CurrentRange(CurrentRanges.cr1uA); //Starts equilabration in the 1µA current range
            _methodCV.Ranging.MinimumCurrentRange = new CurrentRange(CurrentRanges.cr100nA); //Min current range 10nA
            _methodCV.Ranging.MaximumCurrentRange = new CurrentRange(CurrentRanges.cr5mA); //Max current range 1mA
        }

        /// <summary>
        /// Initializes the plot control.
        /// </summary>
        private void InitPlot()
        {
            plot.ClearAll(); //Clear all curves and data from plot
            //Set the Axis labels
            plot.XAxisLabel = "Potential (V)";
            plot.YAxisLabel = "Current (µA)";
            plot.AddData("", new double[0], new double[0]); //Add a empty data array to draw an empty plot
        }

        /// <summary>
        /// Discovers the connected PalmSens & EmStat devices and adds them to the combobox control.
        /// </summary>
        private async Task DiscoverConnectedDevicesAsync()
        {
            btnRefresh.Enabled = false;
            lbConsole.Items.Add("正在搜索可用设备。");
            cmbDevices.Items.Clear();
            //_connectedDevices = await psCommSimpleWinForms.GetConnectedDevicesAsync(); //Discover connected devices
            _connectedDevices = PSSDKBasicAsyncExample1._connectedDevices;

            foreach (Device d in _connectedDevices)
                cmbDevices.Items.Add(d.ToString()); //Add connected devices to control

            int nDevices = cmbDevices.Items.Count;
            cmbDevices.SelectedIndex = nDevices > 0 ? 0 : -1;
            lbConsole.Items.Add($"发现 {nDevices} 台设备。");

            btnConnect.Enabled = nDevices > 0;
            btnRefresh.Enabled = true;
        }

        /// <summary>
        /// Handles the Click event of the btnRefresh control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private async void btnRefresh_Click(object sender, EventArgs e)
        {
            await DiscoverConnectedDevicesAsync(); //Add connected devices to the devices combobox control
        }

        /// <summary>
        /// Handles the Click event of the btnConnect control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private async void btnConnect_Click(object sender, EventArgs e)
        {
            btnConnect.Enabled = false;
            if (!PSSDKBasicAsyncExample1.psCommSimpleWinForms.Connected) //Determine whether a device is currently connected
            {
                if (cmbDevices.SelectedIndex == -1)
                    return;

                try
                {
                    //Connect to the device selected in the devices combobox control
                    await PSSDKBasicAsyncExample1.psCommSimpleWinForms.ConnectAsync(_connectedDevices[cmbDevices.SelectedIndex]); 
                    lbConsole.Items.Add($"已连接到 {PSSDKBasicAsyncExample1.psCommSimpleWinForms.ConnectedDevice.ToString()}");
                }
                catch (Exception ex)
                {
                    lbConsole.Items.Add(ex.Message);
                }
            }
            else
            {
                await PSSDKBasicAsyncExample1.psCommSimpleWinForms.DisconnectAsync(); //Disconnect from the connected device
            }

            //Update UI based on connection status
            cmbDevices.Enabled = !PSSDKBasicAsyncExample1.psCommSimpleWinForms.Connected;
            btnRefresh.Enabled = !PSSDKBasicAsyncExample1.psCommSimpleWinForms.Connected;
            btnConnect.Text = PSSDKBasicAsyncExample1.psCommSimpleWinForms.Connected ? "断开连接" : "连接";
            btnMeasure.Enabled = PSSDKBasicAsyncExample1.psCommSimpleWinForms.Connected;
            btnConnect.Enabled = true;
        }

        /// <summary>
        /// Handles the Click event of the btnMeasure control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private async void btnMeasure_Click(object sender, EventArgs e)
        {
            btnMeasure.Enabled = false;
            if (PSSDKBasicAsyncExample1.psCommSimpleWinForms.DeviceState == PalmSens.Comm.CommManager.DeviceState.Idle) //Determine whether the device is currently idle or measuring
            {
                try
                {
                    plot.ClearAll(); //Clears data from previous measurements from the plot
                    //InitDataGrid();
                    InitCVMethod();
                    _activeMeasurement = await PSSDKBasicAsyncExample1.psCommSimpleWinForms.MeasureAsync(_methodCV); //Start measurement defined in the method
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
                    await PSSDKBasicAsyncExample1.psCommSimpleWinForms.AbortMeasurementAsync(); //Abort the active measurement
                }
                catch (Exception ex)
                {
                    lbConsole.Items.Add(ex.Message);
                }
            }
            btnMeasure.Enabled = true;
        }

        /// <summary>
        /// Handles the Click event of the btnPeaks control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private async void btnPeaks_Click(object sender, EventArgs e)
        {
            btnPeaks.Enabled = false;
            if (_activeMeasurement != null)
                await DetectLSVCVPeaksAsync();
            btnPeaks.Enabled = true;
        }

        /// <summary>
        /// Detects the LSVCV peaks after a measurement is finished.  
        /// 测量结束后检测LSVCV峰值。
        /// </summary>
        private async Task DetectLSVCVPeaksAsync()
        {
            //Create an instance of the class used to detect LSV / CV peaks
            //创建一个用于检测LSV / CV峰值的类的实例
            SemiDerivativePeakDetection semiDerivativePeakDetection = new SemiDerivativePeakDetection();

            //Create an instance of the peak detection progress reporter (this can also be used to cancel the peak detection)
            //创建一个峰值检测进度报告实例(这也可以用来取消峰值检测)
            PeakDetectProgress peakDetectProgress = new PeakDetectProgress();
            peakDetectProgress.ProgressChanged += PeakDetectProgress_ProgressChanged;

            //Add curves from the last measurement and corresponding min peak heights 
            //to the dictionary that is passed on to LSV / CV peak detection class
            //添加上次测量值的曲线和对应的最小峰高
            //传递给LSV / CV峰值检测类的字典
            Dictionary<Curve, double> curves = new Dictionary<Curve, double>();
            foreach (Curve c in _activeMeasurement.SimpleCurveCollection.Select(sc => sc.Curve))
                curves.Add(c, _minPeakHeight);

            SimpleCurve activeSimpleCurve = _activeMeasurement.SimpleCurveCollection[0];

            //Start the semiderivative peak detection (alternatively this can be called synchronously without the PeakDetectProgress object
            //but this could block the UI for quite some time)
            //启动半导数峰值检测(也可以在不使用PeakDetectProgress对象的情况下同步调用
            //但这会阻塞UI相当长一段时间)
            await semiDerivativePeakDetection.GetNonOverlappingPeaksAsync(curves, peakDetectProgress);
        }

        /// <summary>
        /// Raised when the peak detect progress has changed.
        /// 在峰值检测进度发生变化时引发。
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The e.</param>
        private void PeakDetectProgress_ProgressChanged(object sender, PeakDetectProgressUpdate e)
        {
            switch (e.Progress)
            {
                case EnumPeakDetectProgress.Started:
                    lbConsole.Items.Add($"Peak detection started. {e.NCurves} curves remaining.");
                    break;
                case EnumPeakDetectProgress.PeakDetected:
                    lbConsole.Items.Add($"{e.NPeaksFound} peaks found.");
                    break;
                case EnumPeakDetectProgress.ProcessingCurve:
                    lbConsole.Items.Add($"Detecting peaks in {e.CurveTitle}.");
                    break;
                case EnumPeakDetectProgress.CurveProcessed:
                    lbConsole.Items.Add($"{e.NRemainingCurves} curves remaining.");
                    break;
                case EnumPeakDetectProgress.Cancelled:
                    break;
                case EnumPeakDetectProgress.Finished:
                    lbConsole.Items.Add("Completed detecting peaks.");
                    //Draw the peaks in the plot
                    plot.UpdateSimpleCurvesPeaks(_activeMeasurement.SimpleCurveCollection);
                    break;
            }
        }

        /// <summary>
        /// Raised when device status package is received (the device does not send status packages while measuring)
        /// 当接收到设备状态包时引发(设备在测量时不发送状态包)
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
        /// 当连接设备的状态改变时引发
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="CurrentState">State of the current.</param>
        private void psCommSimpleWinForms_StateChanged(object sender, PalmSens.Comm.CommManager.DeviceState CurrentState)
        {
            tbDeviceStatus.Text = CurrentState.ToString(); //Updates the device state indicator textbox
            btnConnect.Enabled = CurrentState == PalmSens.Comm.CommManager.DeviceState.Idle;
            btnMeasure.Text = CurrentState == PalmSens.Comm.CommManager.DeviceState.Idle ? "开始测量" : "取消测量";
        }

        /// <summary>
        /// Raised when the measurement is started
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void psCommSimpleWinForms_MeasurementStarted(object sender, EventArgs e)
        {
            lbConsole.Items.Add("CV开始测量。");
            btnPeaks.Enabled = false;
        }

        /// <summary>
        /// Raised when the measurement is ended
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void psCommSimpleWinForms_MeasurementEnded(object sender, Exception e)
        {
            lbConsole.Items.Add("测量结束。");
            btnPeaks.Enabled = _activeMeasurement.SimpleCurveCollection.Count > 0;
        }

        /// <summary>
        /// Raised when a Simple Curve in the active SimpleMeasurement starts receiving data
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="activeSimpleCurve">The active simple curve.</param>
        private void psCommSimpleWinForms_SimpleCurveStartReceivingData(object sender, SimpleCurve activeSimpleCurve)
        {
            plot.AddSimpleCurve(activeSimpleCurve);

            //Subscribe to the event indicating when the curve stops receiving new data points
            activeSimpleCurve.NewDataAdded += activeSimpleCurve_NewDataAdded;
            activeSimpleCurve.CurveFinished += activeSimpleCurve_CurveFinished;

            lbConsole.Items.Add("正在测量接收数据...");
        }

        /// <summary>
        /// Raised when new data points are added to the active SimpleCurve
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="PalmSens.Data.ArrayDataAddedEventArgs"/> instance containing the event data.</param>
        private void activeSimpleCurve_NewDataAdded(object sender, PalmSens.Data.ArrayDataAddedEventArgs e)
        {
            if (InvokeRequired) //Data is parsed asynchronously in the case this event was raised on a different thread it must be invoked back onto the UI thread
            {
                BeginInvoke(new PalmSens.Plottables.Curve.NewDataAddedEventHandler(activeSimpleCurve_NewDataAdded), sender, e);
                return;
            }
            SimpleCurve activeSimpleCurve = sender as SimpleCurve;
            int startIndex = e.StartIndex; //The index of the first new data point added to the curve
            int count = e.Count; //The number of new data points added to the curve

            for (int i = startIndex; i < startIndex + count; i++)
            {
                double xValue = activeSimpleCurve.XAxisValue(i); //Get the value on Curve's X-Axis (potential) at the specified index获取曲线x轴上指定索引处的值(势)
                double yValue = activeSimpleCurve.YAxisValue(i); //Get the value on Curve's Y-Axis (current) at the specified index获取曲线y轴(current)上指定索引处的值
                dgvMeasurement.Rows.Add(1);
                dgvMeasurement.Rows[i].Cells[0].Value = (i + 1).ToString();
                dgvMeasurement.Rows[i].Cells[1].Value = xValue.ToString("F2");
                dgvMeasurement.Rows[i].Cells[2].Value = yValue.ToString("E3");
            }

            tbPotential.Text = activeSimpleCurve.XAxisValue(startIndex + count - 1).ToString("F3");
            tbCurrent.Text = activeSimpleCurve.YAxisValue(startIndex + count - 1).ToString("F3");
        }

        /// <summary>
        /// Raised when a SimpleCurve stops receiving new data points
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="System.EventArgs" /> instance containing the event data.</param>
        private void activeSimpleCurve_CurveFinished(object sender, EventArgs e)
        {

            if (InvokeRequired) //Data is parsed asynchronously in the case this event was raised on a different thread it must be invoked back onto the UI thread
            {
                BeginInvoke(new EventHandler(activeSimpleCurve_CurveFinished), sender, e);
                return;
            }
            SimpleCurve activeCurve = sender as SimpleCurve;
            int nDataPointsReceived = activeCurve != null ? activeCurve.NDataPoints : 0;
            lbConsole.Items.Add($"一共接收 {nDataPointsReceived} 数据点。");

            //Unsubscribe from the curves events to avoid memory leaks
            activeCurve.NewDataAdded -= activeSimpleCurve_NewDataAdded;
            activeCurve.CurveFinished -= activeSimpleCurve_CurveFinished;
            lbConsole.Items.Add("数据曲线绘图完成");
        }

        /// <summary>
        /// Raised when the instrument has been disconnected.
        /// If the instrument was disconnected due to a communication the exception is provided.
        /// In the case of a regular disconnect the exception will be set to null.
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
            btnMeasure.Text = "开始测量";
        }

        private void tbBegin_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbBegin.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 5 || intValue < -5)
            {
                tbBegin.Text = "";
                MessageBox.Show("输入的参数不正确。");
                return;
            }

            beginV = intValue;
        }

        private void tbHigh_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbHigh.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 5 || intValue < -5)
            {
                tbHigh.Text = "";
                MessageBox.Show("输入的参数不正确。");
                return;
            }

            highV = intValue;
        }

        private void tbLow_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbLow.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 5 || intValue < -5)
            {
                tbLow.Text = "";
                MessageBox.Show("输入的参数不正确。");
                return;
            }

            lowV = intValue;
        }

        private void tbFinal_TextChanged(object sender, EventArgs e)
        {
            tbFinal.Text = Convert.ToString(beginV);
        }

        private void tbStep_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbStep.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 100001 || intValue <= 0.001)
            {
                tbStep.Text = "";
                MessageBox.Show("输入的参数不正确。");
                return;
            }

            stepV = intValue;
        }

        private void tbRate_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbRate.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 100001 || intValue <= 0)
            {
                tbRate.Text = "";
                MessageBox.Show("输入的参数不正确。");
                return;
            }

            rateV = intValue;
        }

        private void tbEquilib_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbEquilib.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 100001 || intValue <= 0)
            {
                tbEquilib.Text = "";
                MessageBox.Show("输入的参数不正确。");
                return;
            }

            equilibT = intValue;
        }

        private void tbnScan_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbnScan.Text;
            if (strValue == "")
            {

                return;
            }
            int intValue = Convert.ToInt32(strValue);
            if (intValue > 100001 || intValue <= 0)
            {
                tbnScan.Text = "";
                MessageBox.Show("输入的参数不正确。");
                return;
            }

            nScan = intValue;
        }
    }
}
