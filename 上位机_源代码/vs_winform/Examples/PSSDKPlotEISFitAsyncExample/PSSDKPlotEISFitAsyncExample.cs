using System;
using System.Drawing;
using System.Threading.Tasks;
using System.Windows.Forms;
using PalmSens;
using PalmSens.Comm;
using PalmSens.Core.Simplified.Data;
using PalmSens.Devices;
using PalmSens.Fitting;
using PalmSens.Fitting.Models;
using PalmSens.Techniques;
using PalmSens.Techniques.Impedance;
//using impedance_winform;
using PSSDKBasicAsyncExample;

namespace PSSDKPlotEISFitAsyncExample
{
    public partial class PSSDKPlotEISFitAsyncExample : Form
    {
        public PSSDKPlotEISFitAsyncExample()
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

            InitEISMethod(); //Create the cyclic voltammetry method that defines the measurement parameters//创建循环伏安法来定义测量参数
            InitDataGrid(); //Set up the columns for the datagridview control
            InitPlot(); //Resets and initiates the plot control
            DiscoverConnectedDevicesAsync(); //Populate the connected device combobox control
        }

        ZoomInWinForm ZoomInWinFormObj = new ZoomInWinForm();

        /// <summary>
        /// The instance of method class containing the Cyclic Voltammetry parameters
        /// </summary>
        private ImpedimetricMethod _methodEIS;

        /// <summary>
        /// The connected PalmSens & EmStat devices
        /// </summary>
        private Device[] _connectedDevices = new Device[0];

        /// <summary>
        /// The active SimpleMeasurement
        /// </summary>
        private SimpleMeasurement _activeMeasurement = null;
        public SimpleCurve activeSimpleCurveBodeZ;
        public SimpleCurve activeSimpleCurveBodeP;
        public SimpleCurve activeSimpleCurveNyquist;
        /// <summary>
        /// The fit result SimpleCurve
        /// </summary>
        private SimpleCurve[] _fitResultCurves = null;

        /// <summary>
        /// The fit progress instance, reports progress of active fit and allows cancellation
        /// </summary>
        private FitProgress _fitProgress = null;

        /// <summary>
        /// The circuit model class defining the model that will be used for the equivalent circuit fit
        /// </summary>
        private CircuitModel _circuitModel = null;


        /// <summary>
        /// Initializes the data grid view control.初始化数据网格视图控件。
        /// </summary>
        private void InitDataGrid()
        {
            dgvMeasurement.Rows.Clear();
            dgvMeasurement.Columns.Clear();

            DataGridViewTextBoxColumn dgvColID = new DataGridViewTextBoxColumn();
            dgvColID.HeaderText = "ID";
            dgvColID.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
            dgvColID.ReadOnly = true;

            DataGridViewTextBoxColumn dgvColFrequency = new DataGridViewTextBoxColumn();
            dgvColFrequency.HeaderText = "Frequency (Hz)";
            dgvColFrequency.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            dgvColFrequency.ReadOnly = true;

            DataGridViewTextBoxColumn dgvColZRe = new DataGridViewTextBoxColumn();
            dgvColZRe.HeaderText = "ZRe (Ohm)";
            dgvColZRe.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            dgvColZRe.ReadOnly = true;
        
            DataGridViewTextBoxColumn dgvColZIm = new DataGridViewTextBoxColumn();
            dgvColZIm.HeaderText = "ZIm (Ohm)";
            dgvColZIm.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            dgvColZIm.ReadOnly = true;

            DataGridViewTextBoxColumn dgvColZ = new DataGridViewTextBoxColumn();
            dgvColZ.HeaderText = "Z(Ohm)";
            dgvColZ.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            dgvColZ.ReadOnly = true;

            DataGridViewTextBoxColumn dgvColPhase = new DataGridViewTextBoxColumn();
            dgvColPhase.HeaderText = "Phase ( °)";
            dgvColPhase.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            dgvColPhase.ReadOnly = true;

            dgvMeasurement.Columns.Add(dgvColID);
            dgvMeasurement.Columns.Add(dgvColFrequency);
            dgvMeasurement.Columns.Add(dgvColZRe);
            dgvMeasurement.Columns.Add(dgvColZIm);
            dgvMeasurement.Columns.Add(dgvColZ);
            dgvMeasurement.Columns.Add(dgvColPhase);
        }

        /// <summary>
        /// Initializes the EIS method.
        /// </summary>
        private void InitEISMethod()
        {
            tbDC.Text = Convert.ToString(dcV);
            tbAC.Text = Convert.ToString(acV);
            tbMaxFre.Text = Convert.ToString(maxFre);
            tbMinFre.Text = Convert.ToString(minFre);
            tbNumFre.Text = Convert.ToString(numFre);
            tbStepFre.Text = Convert.ToString(stepFre);
            tbEquilibT.Text = Convert.ToString(equilibT);
            tbFaxFre.Text = Convert.ToString(faxFre);

            _methodEIS = new ImpedimetricMethod();
            _methodEIS.EquilibrationTime = equilibT; //Equilabrates the cell at the defined potential for 5 seconds before starting the measurement在开始测量前，使电池在定义电位下平衡5秒
            _methodEIS.Potential = dcV; //Sets the dc potential
            _methodEIS.Eac = acV; // Sets the ac potential at 10 mV RMS
            _methodEIS.ScanType = enumScanType.Fixed; //Single measurement at fixed frequency
            _methodEIS.FreqType = enumFrequencyType.Scan; //Scan a range of frequencies
            _methodEIS.MaxFrequency = maxFre; //Start scan at 200 000 Hz
            _methodEIS.MinFrequency = minFre; //End scan at 100 Hz
            _methodEIS.nFrequencies = numFre; //Sets number of frequencies in scan to 50

            _methodEIS.Ranging.StartCurrentRange = new CurrentRange(CurrentRanges.cr10mA); //Starts equilabration in the 10mA current range
            _methodEIS.Ranging.MinimumCurrentRange = new CurrentRange(CurrentRanges.cr100nA); //Min current range 1µA
            _methodEIS.Ranging.MaximumCurrentRange = new CurrentRange(CurrentRanges.cr10mA); //Max current range 10mA
        }

        /// <summary>
        /// Initializes the plot control.
        /// </summary>
        private void InitPlot()
        {

            //Nyquist plot
            plotNyquist.ClearAll(); //Clear all curves and data from plot//清除plot中的所有曲线和数据
            //Set the Axis labels//设置Axis标签
            plotNyquist.XAxisLabel = "Z Re";
            plotNyquist.YAxisLabel = "Z Im";
            plotNyquist.AddData("", new double[0], new double[0]); //Add a empty data array to draw an empty plot//添加一个空的数据数组来绘制一个空的图形

            plotBode.ClearAll(); //Clear all curves and data from plot
            //Set the Axis labels
            plotBode.XAxisLabel = "Log Frequency (Hz)";
            plotBode.XAxisType = SDKPlot.AxisType.Logarithmic;
            plotBode.YAxisLabel = "Log Z (Ω)";
            plotBode.YAxisType = SDKPlot.AxisType.Logarithmic;
            plotBode.YAxisSecondaryLabel = "-Phase (°)";
            plotBode.YAxisSecondaryType = SDKPlot.AxisType.Linear;
            plotBode.AddData("", new double[0], new double[0]); //Add a empty data array to draw an empty plot


            ZoomInWinFormObj.plotBodeZoom.ClearAll();
            ZoomInWinFormObj.plotBodeZoom.XAxisLabel = "Log Frequency (Hz)";
            ZoomInWinFormObj.plotBodeZoom.XAxisType = SDKPlot.AxisType.Logarithmic;
            ZoomInWinFormObj.plotBodeZoom.YAxisLabel = "Log Z (Ω)";
            ZoomInWinFormObj.plotBodeZoom.YAxisType = SDKPlot.AxisType.Logarithmic;
            ZoomInWinFormObj.plotBodeZoom.YAxisSecondaryLabel = "-Phase (°)";
            ZoomInWinFormObj.plotBodeZoom.YAxisSecondaryType = SDKPlot.AxisType.Linear;
            ZoomInWinFormObj.plotBodeZoom.AddData("", new double[0], new double[0]);

            ZoomInWinFormObj.plotNyquistZoom.ClearAll();
            ZoomInWinFormObj.plotNyquistZoom.XAxisLabel = "Z Re";
            ZoomInWinFormObj.plotNyquistZoom.YAxisLabel = "Z Im";
            ZoomInWinFormObj.plotNyquistZoom.AddData("", new double[0], new double[0]);

        }

        /// <summary>
        /// Discovers the connected PalmSens & EmStat devices and adds them to the combobox control.
        /// </summary>
        private async Task DiscoverConnectedDevicesAsync()
        {
            btnRefresh.Enabled = false;
            lbConsole.Items.Add("正在搜索可用设备.");
            cmbDevices.Items.Clear();
            //_connectedDevices = await psCommSimpleWinForms.GetConnectedDevicesAsync(); //Discover connected devices
            _connectedDevices = PSSDKBasicAsyncExample1._connectedDevices;

            foreach (Device d in _connectedDevices)
                cmbDevices.Items.Add(d.ToString()); //Add connected devices to control

            int nDevices = cmbDevices.Items.Count;
            cmbDevices.SelectedIndex = nDevices > 0 ? 0 : -1;
            lbConsole.Items.Add($"发现 {nDevices} 台设备.");

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
                    plotBode.ClearAll(); //Clears data from previous measurements from the plot
                    plotNyquist.ClearAll();
                    InitEISMethod();
                    _activeMeasurement = await PSSDKBasicAsyncExample1.psCommSimpleWinForms.MeasureAsync(_methodEIS); //Start measurement defined in the method

                    activeSimpleCurveBodeZ = _activeMeasurement.NewSimpleCurve(PalmSens.Data.DataArrayType.Frequency, PalmSens.Data.DataArrayType.Z, "Z Abs")[0];
                    activeSimpleCurveBodeP = _activeMeasurement.NewSimpleCurve(PalmSens.Data.DataArrayType.Frequency, PalmSens.Data.DataArrayType.Phase, "-Phase")[0];
                    activeSimpleCurveNyquist = _activeMeasurement.NewSimpleCurve(PalmSens.Data.DataArrayType.ZRe, PalmSens.Data.DataArrayType.ZIm, "Nyquist")[0];
                    //Add bode curves to plot
                    plotBode.AddSimpleCurve(activeSimpleCurveBodeZ);
                    plotBode.AddSimpleCurve(activeSimpleCurveBodeP, true);
                    plotNyquist.AddSimpleCurve(activeSimpleCurveNyquist);

                    ZoomInWinFormObj.plotBodeZoom.AddSimpleCurve(activeSimpleCurveBodeZ);
                    ZoomInWinFormObj.plotBodeZoom.AddSimpleCurve(activeSimpleCurveBodeP, true);
                    ZoomInWinFormObj.plotNyquistZoom.AddSimpleCurve(activeSimpleCurveNyquist);
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
        /// Raised when a Simple Curve in the active SimpleMeasurement starts receiving data当活动simplemmeasurement中的简单曲线开始接收数据时引发
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="activeSimpleCurve">The active simple curve.</param>
        private void psCommSimpleWinForms_SimpleCurveStartReceivingData(object sender, SimpleCurve activeSimpleCurve)
        {
            //plot.AddSimpleCurve(activeSimpleCurve);

            //Only add default Nyquist curve to plot here, the bode plot curves have already been added above
            //这里只添加默认的Nyquist曲线来绘图，博德曲线已经在上面添加了
            if (activeSimpleCurve.Title == "Bode")
            {
                lbConsole.Items.Add("没有默认Nyquist");
                return;
            }
                

            //plotNyquist.AddSimpleCurve(activeSimpleCurve);
            //Subscribe to the curve's events to receive updates when new data is available and when it iss finished receiving data
            //订阅曲线的事件，以便在新数据可用和接收数据完成时接收更新
            activeSimpleCurve.NewDataAdded += activeSimpleCurve_NewDataAdded;
            activeSimpleCurve.CurveFinished += activeSimpleCurve_CurveFinished;

            lbConsole.Items.Add("正在接收新的数据...");
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
                
        
         
                double Fre = activeSimpleCurveBodeZ.XAxisValue(i);
                double ZRe = activeSimpleCurveNyquist.XAxisValue(i); //Get the value on Curve's X-Axis (potential) at the specified index获取曲线x轴上指定索引处的值(势)
                double ZIm = activeSimpleCurveNyquist.YAxisValue(i); //Get the value on Curve's Y-Axis (current) at the specified index获取曲线y轴(current)上指定索引处的值
                double Z = activeSimpleCurveBodeZ.YAxisValue(i);
                double P = activeSimpleCurveBodeP.YAxisValue(i);
                dgvMeasurement.Rows.Add(1);
                dgvMeasurement.Rows[i].Cells[0].Value = (i + 1).ToString();
                dgvMeasurement.Rows[i].Cells[1].Value = Fre.ToString("F2");
                dgvMeasurement.Rows[i].Cells[2].Value = ZRe.ToString("E3");
                dgvMeasurement.Rows[i].Cells[3].Value = ZIm.ToString("E3");
                dgvMeasurement.Rows[i].Cells[4].Value = Z.ToString("E3");
                dgvMeasurement.Rows[i].Cells[5].Value = P.ToString("E3");
            }

            tbPotential.Text = activeSimpleCurve.XAxisValue(startIndex + count - 1).ToString("F3");
            tbCurrent.Text = activeSimpleCurve.YAxisValue(startIndex + count - 1).ToString("F3");
        }

        /// <summary>
        /// Raised when a SimpleCurve stops receiving new data points当SimpleCurve停止接收新数据点时引发
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void activeSimpleCurve_CurveFinished(object sender, EventArgs e)
        {
            if (InvokeRequired) //Data is parsed asynchronously in the case this event was raised on a different thread it must be invoked back onto the UI thread如果在不同的线程上引发此事件，则数据将被异步解析，必须将其调用回UI线程
            {
                BeginInvoke(new EventHandler(activeSimpleCurve_CurveFinished), sender, e);
                return;
            }
            SimpleCurve activeSimpleCurve = sender as SimpleCurve;

            //Unsubscribe from the curves events to avoid memory leaks取消订阅曲线事件以避免内存泄漏
            activeSimpleCurve.NewDataAdded -= activeSimpleCurve_NewDataAdded;
            activeSimpleCurve.CurveFinished -= activeSimpleCurve_CurveFinished;

            int nDataPointsReceived = activeSimpleCurve.NDataPoints;
            lbConsole.Items.Add($"一共接收 {nDataPointsReceived}  数据点.");

            lbConsole.Items.Add("数据曲线绘图完成");
        }














































        /// <summary>
        /// Handles the Click event of the btnFit control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private async void btnFit_Click(object sender, EventArgs e)
        {
            btnFit.Enabled = false;
            await FitEquivalentCircuitAsync(); //Fits a specified equivalent circuit to the measured data
            btnFit.Enabled = true;
        }

        /// <summary>
        /// Handles the Click event of the btnCancel control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void btnCancel_Click(object sender, EventArgs e)
        {
            if (_fitProgress != null) _fitProgress.Cancel(); //Cancels the active fit
        }

        /// <summary>
        /// Fits the equivalent circuit.
        /// </summary>
        /// <exception cref="System.NotImplementedException"></exception>
        private async Task FitEquivalentCircuitAsync()
        {
            _circuitModel = BuildEquivalentCircuitModel(); //Build the model to fit on the data
            FitOptionsCircuit fitOptions = InitFitOptions(_circuitModel); //Define the options for the fit
            FitAlgorithm fitAlgorithm = FitAlgorithm.FromAlgorithm(fitOptions); //Set up the instance of the fit class
            _fitProgress = new FitProgress(); //Create an instance of the peak detection progress reporter (this can also be used to cancel the peak detection)
            _fitProgress.ProgressChanged += FitProgress_ProgressChanged;
            await fitAlgorithm.ApplyFitCircuitAsync(_fitProgress); //Start the fit
        }

        /// <summary>
        /// Builds the equivalent circuit model.
        /// </summary>
        /// <returns></returns>
        private CircuitModel BuildEquivalentCircuitModel()
        {
            CircuitModel circuitModel = new CircuitModel();
            circuitModel.SetEISdata(_activeMeasurement.Measurement.EISdata[0]); //Sets reference to measured data
            circuitModel.SetCircuit("R(RC)"); //Sets the circuit defined in the CDC code string, in this case a Randles circuit
            circuitModel.SetInitialParameters(
                new double[] {
                    100, //The initial value for the solution resistance (series resistor)
                    8000, //The initial value for the charge transfer resistance (parallel resistor)
                    1e-8 //The initial value for the double layer capacitance (parallel capacitor)
                });
            //Setting the initial parameters is recommended to ensure a good quality fit and 
            //reduce to the time it takes to find the fit

            return circuitModel;
        }

        /// <summary>
        /// Initializes the fit options.
        /// </summary>
        /// <param name="circuitModel">The circuit model.</param>
        /// <returns></returns>
        private FitOptionsCircuit InitFitOptions(CircuitModel circuitModel)
        {
            FitOptionsCircuit fitOptions = new FitOptionsCircuit();
            fitOptions.Model = circuitModel;
            fitOptions.RawData = _activeMeasurement.Measurement.EISdata[0]; //The measured data to fit onto
            fitOptions.SelectedAlgorithm = Algorithm.LevenbergMarquardt;
            fitOptions.MaxIterations = 100; //The maximum number of iterations, 500 by default
            fitOptions.MinimumDeltaErrorTerm = 1e-9; //The minimum delta in the error term (sum of squares difference between model and data), default is 1e-9      
            fitOptions.EnableMinimunDeltaErrorTerm = true; //Enable the minimum delta error as a stop condition within the fit algorithm, default is true
            fitOptions.MinimumDeltaParameters = 1e-12; //The minimum delta parameter step size, default is 1e-12
            fitOptions.EnableMinimunDeltaParameters = true; //Enable the minimum delta parameter step size as a stop condition within the fit algorithm, default is true
            fitOptions.Lambda = 1e-2; //The starting value for the Levenberg Marquardt scaling factor, default is 1e-2
            fitOptions.LambdaFactor = 10; //The scaling value for the Levenberg Marquardt scaling factor, default is 10

            return fitOptions;
        }

        /// <summary>
        /// Raised when fit progress changed.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The e.</param>
        /// <exception cref="NotImplementedException"></exception>
        private void FitProgress_ProgressChanged(object sender, FitProgressUpdate e)
        {
            switch (e.Progress)
            {
                case EnumFitProgress.Started:
                    lbConsole.Items.Add("Started circuit fit.");
                    lblIterations.Text = "";
                    btnCancel.Enabled = true;
                    progressBar.MarqueeAnimationSpeed = 100;
                    break;
                case EnumFitProgress.FitIterated:
                    lblIterations.Text = e.NIterations.ToString();
                    break;
                case EnumFitProgress.Cancelled:
                case EnumFitProgress.Finished:
                    btnCancel.Enabled = false;
                    progressBar.MarqueeAnimationSpeed = 0;
                    lblIterations.Text = (e.Result.NIterations - 1).ToString(); //Excludes initial guess as iteration
                    SetFitResults(e.Result);
                    lbConsole.Items.Add("Circuit fit completed.");
                    _fitProgress.ProgressChanged -= FitProgress_ProgressChanged;
                    _fitProgress = null;
                    break;
            }
        }

        /// <summary>
        /// Sets the fit results.
        /// </summary>
        /// <param name="result">The result.</param>
        private void SetFitResults(FitResult result)
        {
            //Add results to console
            lbResults.Items.Clear();
            lbResults.Items.Add($"Rs = {result.FinalParameters[0].ToString("0.00E+000")} Ω");
            lbResults.Items.Add($"%error = {result.ParameterSDs[0].ToString("0.00E+000")} %");
            lbResults.Items.Add($"Rct = {result.FinalParameters[1].ToString("0.00E+000")} Ω");
            lbResults.Items.Add($"%error = {result.ParameterSDs[1].ToString("0.00E+000")} %");
            lbResults.Items.Add($"Cdl = {result.FinalParameters[2].ToString("0.00E+000")} F");
            lbResults.Items.Add($"%error = {result.ParameterSDs[2].ToString("0.00E+000")} %");
            lbResults.Items.Add($"Chi² = {result.ChiSq.ToString("0.00E+000")}");
            switch (result.ExitCode)
            {
                case ExitCodes.MinimumDeltaErrorTerm:
                    lbResults.Items.Add("Reached min delta error sum squares");
                    break;
                case ExitCodes.MinimumDeltaParameters:
                    lbResults.Items.Add("Reached min delta parameter step size");
                    break;
                case ExitCodes.MaxIterations:
                    lbResults.Items.Add("Reached max iterations");
                    break;
                case ExitCodes.HessianNonPositive:
                    lbResults.Items.Add("Error determing step for next iteration");
                    break;
            }

            //Remove previous fit results from plot
            if (_fitResultCurves != null)
                foreach (SimpleCurve sc in _fitResultCurves)
                    if (plotBode.ContainsSimpleCurve(sc))
                        plotBode.RemoveSimpleCurve(sc);

            //Generate new simple curves based on fit results
            _circuitModel.SetInitialParameters(result.FinalParameters); //Update the initial parameters with fit results
            _fitResultCurves = new SimpleCurve[3];
            _fitResultCurves[0] = new SimpleCurve(_circuitModel.GetCurveZabsOverFrequency(false)[0], _activeMeasurement);
            _fitResultCurves[0].Title = "Z Abs Fit";
            _fitResultCurves[1] = new SimpleCurve(_circuitModel.GetCurvePhaseOverFrequency(false)[0], _activeMeasurement);
            _fitResultCurves[1].Title = "-Phase Fit";
            _fitResultCurves[2] = new SimpleCurve(_circuitModel.GetNyquist()[0], _activeMeasurement);
            _fitResultCurves[2].Title = "Nyquist Fit";
           
            //Add results to plot   
            plotBode.AddSimpleCurve(_fitResultCurves[0], false, false);
            plotBode.AddSimpleCurve(_fitResultCurves[1], true, true);
            plotNyquist.AddSimpleCurve(_fitResultCurves[2], false, true);
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
            btnMeasure.Text = CurrentState == PalmSens.Comm.CommManager.DeviceState.Idle ? "开始测量" : "取消测量";
        }

        /// <summary>
        /// Raised when the measurement is started
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void psCommSimpleWinForms_MeasurementStarted(object sender, EventArgs e)
        {
            lbConsole.Items.Add("EIS 测量开始.");
            btnFit.Enabled = false;
        }

        /// <summary>
        /// Raised when the measurement is ended
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void psCommSimpleWinForms_MeasurementEnded(object sender, Exception e)
        {
            lbConsole.Items.Add("测量结束");
            btnFit.Enabled = _activeMeasurement.Measurement.EISdata.Count > 0 && _activeMeasurement.Measurement.EISdata[0].NPoints > 0;
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
            btnMeasure.Text = "测量";
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label12_Click(object sender, EventArgs e)
        {

        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void label11_Click(object sender, EventArgs e)
        {

        }

        private void tbDC_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbDC.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 2 || intValue < -2)
            {
                lbConsole.Items.Add("输入的参数不正确，");
                lbConsole.Items.Add("最大步进电位为0.250V。");
                MessageBox.Show("输入的参数不正确，最大步进电位为0.250V。");
                tbDC.Text = "";

                return;
            }

            dcV = intValue;
        }

        private void tbAC_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbAC.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 0.006)
            {
                lbConsole.Items.Add("输入的参数不正确，");
                lbConsole.Items.Add("电流范围在100nA时，");
                lbConsole.Items.Add("最大交流振幅为6mV。");
                MessageBox.Show("输入的参数不正确，电流范围在100nA时，最大交流振幅为6mV。");
                tbAC.Text = "";

                return;
            }
            if (intValue < 0)
            {
                lbConsole.Items.Add("输入的参数不正确，");
                lbConsole.Items.Add("确保交流振幅有值。");
                MessageBox.Show("输入的参数不正确，确保交流振幅有值。");
                tbAC.Text = "";

                return;
            }
            acV = intValue;
        }

        private void tbMaxFre_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbMaxFre.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 200000 || intValue < 0.016)
            {
                lbConsole.Items.Add("输入的参数不正确，");
                lbConsole.Items.Add("频率范围为0.016--200000Hz。");
                MessageBox.Show("输入的参数不正确，频率范围为0.016--200000Hz。");
                tbMaxFre.Text = "";

                return;
            }

            maxFre = intValue;

            //tbNumFre_TextChanged(sender, e);
        }

        private void tbMinFre_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbMinFre.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 200000 || intValue < 0.016)
            {
                lbConsole.Items.Add("输入的参数不正确，");
                lbConsole.Items.Add("频率范围为0.016--200000Hz。");
                MessageBox.Show("输入的参数不正确，频率范围为0.016--200000Hz。");
                tbMinFre.Text = "";

                return;
            }

            minFre = intValue;

            tbNumFre_TextChanged(sender, e);
        }

        private void tbEquilibT_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbEquilibT.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);

            equilibT = intValue;
            tbEquilibT.Text = Convert.ToString(equilibT);
        }

        private void tbNumFre_TextChanged(object sender, EventArgs e)
        {

            string strValue = tbNumFre.Text;
            if (strValue == "")
            {

                return;
            }
            int intValue = Convert.ToInt32(strValue);
            if (intValue < 2 || intValue > 501)
            {
                tbNumFre.Text = "";
                MessageBox.Show("输入的参数不正确，最大数量为501。");
                return;
            }
            stepFre = (maxFre - minFre)/(intValue - 1);
            tbStepFre.Text = Convert.ToString(stepFre);
        }

        private void tbStepFre_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbStepFre.Text;
            if (strValue == "")
            {
               
                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 100001 || intValue == 0 )
            {
                tbStepFre.Text = "";
                MessageBox.Show("输入的参数不正确。");
                return;
            }
            numFre = Convert.ToInt32( (maxFre - minFre)/intValue + 1 );
            
            tbNumFre.Text = Convert.ToString(numFre);

            

        }

        private void btBodeZoomin_Click(object sender, EventArgs e)
        {
            ZoomInWinFormObj.ShowDialog();
        }
    }
}
