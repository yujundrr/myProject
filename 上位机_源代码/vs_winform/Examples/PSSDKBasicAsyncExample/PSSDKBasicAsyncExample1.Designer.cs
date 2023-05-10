using System;

namespace PSSDKBasicAsyncExample
{
     partial class PSSDKBasicAsyncExample1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        public void InitializeComponent()
        {
            this.cmbDevices = new System.Windows.Forms.ComboBox();
            this.btnRefresh = new System.Windows.Forms.Button();
            this.btnConnect = new System.Windows.Forms.Button();
            this.btnMeasure = new System.Windows.Forms.Button();
            this.grpConnection = new System.Windows.Forms.GroupBox();
            this.grpDevice = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tbDeviceStatus = new System.Windows.Forms.TextBox();
            this.lblCurrentRange = new System.Windows.Forms.Label();
            this.lblCurrent = new System.Windows.Forms.Label();
            this.tbCurrent = new System.Windows.Forms.TextBox();
            this.lblVolt = new System.Windows.Forms.Label();
            this.tbPotential = new System.Windows.Forms.TextBox();
            this.lblPotential = new System.Windows.Forms.Label();
            this.grpMeasurement = new System.Windows.Forms.GroupBox();
            this.dgvMeasurement = new System.Windows.Forms.DataGridView();
            this.grpConsole = new System.Windows.Forms.GroupBox();
            this.lbConsole = new System.Windows.Forms.ListBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.plot = new SDKPlot.WinForms.Plot();
            this.grpParamSet = new System.Windows.Forms.GroupBox();
            this.label9 = new System.Windows.Forms.Label();
            this.tbEquilib = new System.Windows.Forms.TextBox();
            this.labelEquilib = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.tbRate = new System.Windows.Forms.TextBox();
            this.tbStep = new System.Windows.Forms.TextBox();
            this.labelRate = new System.Windows.Forms.Label();
            this.labelStep = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.labelEnd = new System.Windows.Forms.Label();
            this.tbEnd = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.tbBegin = new System.Windows.Forms.TextBox();
            this.labelBegin = new System.Windows.Forms.Label();
            this.grpConnection.SuspendLayout();
            this.grpDevice.SuspendLayout();
            this.grpMeasurement.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvMeasurement)).BeginInit();
            this.grpConsole.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.grpParamSet.SuspendLayout();
            this.SuspendLayout();
            // 
            // cmbDevices
            // 
            this.cmbDevices.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbDevices.FormattingEnabled = true;
            this.cmbDevices.Location = new System.Drawing.Point(6, 18);
            this.cmbDevices.Name = "cmbDevices";
            this.cmbDevices.Size = new System.Drawing.Size(155, 20);
            this.cmbDevices.TabIndex = 0;
            // 
            // btnRefresh
            // 
            this.btnRefresh.Location = new System.Drawing.Point(6, 42);
            this.btnRefresh.Name = "btnRefresh";
            this.btnRefresh.Size = new System.Drawing.Size(75, 21);
            this.btnRefresh.TabIndex = 1;
            this.btnRefresh.Text = "刷新";
            this.btnRefresh.UseVisualStyleBackColor = true;
            this.btnRefresh.Click += new System.EventHandler(this.btnRefresh_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Enabled = false;
            this.btnConnect.Location = new System.Drawing.Point(86, 42);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(75, 21);
            this.btnConnect.TabIndex = 2;
            this.btnConnect.Text = "连接";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // btnMeasure
            // 
            this.btnMeasure.Enabled = false;
            this.btnMeasure.Location = new System.Drawing.Point(6, 18);
            this.btnMeasure.Name = "btnMeasure";
            this.btnMeasure.Size = new System.Drawing.Size(75, 21);
            this.btnMeasure.TabIndex = 3;
            this.btnMeasure.Text = "开始测量";
            this.btnMeasure.UseVisualStyleBackColor = true;
            this.btnMeasure.Click += new System.EventHandler(this.btnMeasure_Click);
            // 
            // grpConnection
            // 
            this.grpConnection.Controls.Add(this.cmbDevices);
            this.grpConnection.Controls.Add(this.btnRefresh);
            this.grpConnection.Controls.Add(this.btnConnect);
            this.grpConnection.Location = new System.Drawing.Point(12, 9);
            this.grpConnection.Name = "grpConnection";
            this.grpConnection.Size = new System.Drawing.Size(171, 73);
            this.grpConnection.TabIndex = 4;
            this.grpConnection.TabStop = false;
            this.grpConnection.Text = "设备连接";
            // 
            // grpDevice
            // 
            this.grpDevice.Controls.Add(this.label1);
            this.grpDevice.Controls.Add(this.tbDeviceStatus);
            this.grpDevice.Controls.Add(this.lblCurrentRange);
            this.grpDevice.Controls.Add(this.lblCurrent);
            this.grpDevice.Controls.Add(this.tbCurrent);
            this.grpDevice.Controls.Add(this.lblVolt);
            this.grpDevice.Controls.Add(this.tbPotential);
            this.grpDevice.Controls.Add(this.lblPotential);
            this.grpDevice.Location = new System.Drawing.Point(190, 9);
            this.grpDevice.Name = "grpDevice";
            this.grpDevice.Size = new System.Drawing.Size(296, 73);
            this.grpDevice.TabIndex = 5;
            this.grpDevice.TabStop = false;
            this.grpDevice.Text = "设备状态";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(189, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 12);
            this.label1.TabIndex = 7;
            this.label1.Text = "状态:";
            // 
            // tbDeviceStatus
            // 
            this.tbDeviceStatus.BackColor = System.Drawing.SystemColors.Control;
            this.tbDeviceStatus.ForeColor = System.Drawing.SystemColors.GrayText;
            this.tbDeviceStatus.Location = new System.Drawing.Point(192, 42);
            this.tbDeviceStatus.Name = "tbDeviceStatus";
            this.tbDeviceStatus.ReadOnly = true;
            this.tbDeviceStatus.Size = new System.Drawing.Size(98, 21);
            this.tbDeviceStatus.TabIndex = 6;
            // 
            // lblCurrentRange
            // 
            this.lblCurrentRange.AutoSize = true;
            this.lblCurrentRange.Location = new System.Drawing.Point(140, 44);
            this.lblCurrentRange.Name = "lblCurrentRange";
            this.lblCurrentRange.Size = new System.Drawing.Size(47, 12);
            this.lblCurrentRange.TabIndex = 5;
            this.lblCurrentRange.Text = "* 10 mA";
            // 
            // lblCurrent
            // 
            this.lblCurrent.AutoSize = true;
            this.lblCurrent.Location = new System.Drawing.Point(7, 44);
            this.lblCurrent.Name = "lblCurrent";
            this.lblCurrent.Size = new System.Drawing.Size(35, 12);
            this.lblCurrent.TabIndex = 4;
            this.lblCurrent.Text = "电流:";
            // 
            // tbCurrent
            // 
            this.tbCurrent.BackColor = System.Drawing.SystemColors.Control;
            this.tbCurrent.ForeColor = System.Drawing.SystemColors.GrayText;
            this.tbCurrent.Location = new System.Drawing.Point(64, 42);
            this.tbCurrent.Name = "tbCurrent";
            this.tbCurrent.ReadOnly = true;
            this.tbCurrent.Size = new System.Drawing.Size(70, 21);
            this.tbCurrent.TabIndex = 3;
            // 
            // lblVolt
            // 
            this.lblVolt.AutoSize = true;
            this.lblVolt.Location = new System.Drawing.Point(140, 20);
            this.lblVolt.Name = "lblVolt";
            this.lblVolt.Size = new System.Drawing.Size(11, 12);
            this.lblVolt.TabIndex = 2;
            this.lblVolt.Text = "V";
            // 
            // tbPotential
            // 
            this.tbPotential.BackColor = System.Drawing.SystemColors.Control;
            this.tbPotential.ForeColor = System.Drawing.SystemColors.GrayText;
            this.tbPotential.Location = new System.Drawing.Point(64, 18);
            this.tbPotential.Name = "tbPotential";
            this.tbPotential.ReadOnly = true;
            this.tbPotential.Size = new System.Drawing.Size(70, 21);
            this.tbPotential.TabIndex = 1;
            // 
            // lblPotential
            // 
            this.lblPotential.AutoSize = true;
            this.lblPotential.Location = new System.Drawing.Point(7, 20);
            this.lblPotential.Name = "lblPotential";
            this.lblPotential.Size = new System.Drawing.Size(35, 12);
            this.lblPotential.TabIndex = 0;
            this.lblPotential.Text = "电压:";
            // 
            // grpMeasurement
            // 
            this.grpMeasurement.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.grpMeasurement.Controls.Add(this.dgvMeasurement);
            this.grpMeasurement.Controls.Add(this.btnMeasure);
            this.grpMeasurement.Location = new System.Drawing.Point(190, 89);
            this.grpMeasurement.Name = "grpMeasurement";
            this.grpMeasurement.Size = new System.Drawing.Size(472, 358);
            this.grpMeasurement.TabIndex = 6;
            this.grpMeasurement.TabStop = false;
            this.grpMeasurement.Text = "测量数据";
            // 
            // dgvMeasurement
            // 
            this.dgvMeasurement.AllowUserToAddRows = false;
            this.dgvMeasurement.AllowUserToDeleteRows = false;
            this.dgvMeasurement.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dgvMeasurement.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvMeasurement.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.dgvMeasurement.Location = new System.Drawing.Point(6, 45);
            this.dgvMeasurement.Name = "dgvMeasurement";
            this.dgvMeasurement.ReadOnly = true;
            this.dgvMeasurement.RowHeadersVisible = false;
            this.dgvMeasurement.Size = new System.Drawing.Size(463, 304);
            this.dgvMeasurement.TabIndex = 4;
            // 
            // grpConsole
            // 
            this.grpConsole.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.grpConsole.Controls.Add(this.lbConsole);
            this.grpConsole.Location = new System.Drawing.Point(12, 89);
            this.grpConsole.Name = "grpConsole";
            this.grpConsole.Size = new System.Drawing.Size(172, 358);
            this.grpConsole.TabIndex = 7;
            this.grpConsole.TabStop = false;
            this.grpConsole.Text = "日志打印";
            // 
            // lbConsole
            // 
            this.lbConsole.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.lbConsole.FormattingEnabled = true;
            this.lbConsole.ItemHeight = 12;
            this.lbConsole.Location = new System.Drawing.Point(7, 18);
            this.lbConsole.Name = "lbConsole";
            this.lbConsole.Size = new System.Drawing.Size(154, 328);
            this.lbConsole.TabIndex = 0;
            this.lbConsole.SelectedIndexChanged += new System.EventHandler(this.lbConsole_SelectedIndexChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.plot);
            this.groupBox1.Location = new System.Drawing.Point(686, 89);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(465, 358);
            this.groupBox1.TabIndex = 8;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "绘制曲线";
            // 
            // plot
            // 
            this.plot.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.plot.BackColor = System.Drawing.Color.White;
            this.plot.Location = new System.Drawing.Point(6, 45);
            this.plot.MarkerSize = 5;
            this.plot.MarkerType = OxyPlot.MarkerType.Circle;
            this.plot.Name = "plot";
            this.plot.Size = new System.Drawing.Size(453, 304);
            this.plot.TabIndex = 4;
            this.plot.Title = null;
            this.plot.XAxisLabel = null;
            this.plot.XAxisType = SDKPlot.AxisType.Linear;
            this.plot.YAxisLabel = null;
            this.plot.YAxisSecondaryLabel = null;
            this.plot.YAxisSecondaryType = SDKPlot.AxisType.Linear;
            this.plot.YAxisType = SDKPlot.AxisType.Linear;
            // 
            // grpParamSet
            // 
            this.grpParamSet.Controls.Add(this.label9);
            this.grpParamSet.Controls.Add(this.tbEquilib);
            this.grpParamSet.Controls.Add(this.labelEquilib);
            this.grpParamSet.Controls.Add(this.label7);
            this.grpParamSet.Controls.Add(this.label6);
            this.grpParamSet.Controls.Add(this.tbRate);
            this.grpParamSet.Controls.Add(this.tbStep);
            this.grpParamSet.Controls.Add(this.labelRate);
            this.grpParamSet.Controls.Add(this.labelStep);
            this.grpParamSet.Controls.Add(this.label3);
            this.grpParamSet.Controls.Add(this.labelEnd);
            this.grpParamSet.Controls.Add(this.tbEnd);
            this.grpParamSet.Controls.Add(this.label5);
            this.grpParamSet.Controls.Add(this.tbBegin);
            this.grpParamSet.Controls.Add(this.labelBegin);
            this.grpParamSet.Location = new System.Drawing.Point(505, 12);
            this.grpParamSet.Name = "grpParamSet";
            this.grpParamSet.Size = new System.Drawing.Size(627, 73);
            this.grpParamSet.TabIndex = 8;
            this.grpParamSet.TabStop = false;
            this.grpParamSet.Text = "LSV参数设置";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(524, 24);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(11, 12);
            this.label9.TabIndex = 16;
            this.label9.Text = "s";
            // 
            // tbEquilib
            // 
            this.tbEquilib.Location = new System.Drawing.Point(448, 20);
            this.tbEquilib.Name = "tbEquilib";
            this.tbEquilib.Size = new System.Drawing.Size(70, 21);
            this.tbEquilib.TabIndex = 15;
            this.tbEquilib.TextChanged += new System.EventHandler(this.tbEquilib_TextChanged);
            // 
            // labelEquilib
            // 
            this.labelEquilib.AutoSize = true;
            this.labelEquilib.Location = new System.Drawing.Point(383, 23);
            this.labelEquilib.Name = "labelEquilib";
            this.labelEquilib.Size = new System.Drawing.Size(59, 12);
            this.labelEquilib.TabIndex = 14;
            this.labelEquilib.Text = "静息时间:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(330, 44);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(23, 12);
            this.label7.TabIndex = 13;
            this.label7.Text = "V/s";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(330, 20);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(11, 12);
            this.label6.TabIndex = 12;
            this.label6.Text = "V";
            // 
            // tbRate
            // 
            this.tbRate.Location = new System.Drawing.Point(254, 41);
            this.tbRate.Name = "tbRate";
            this.tbRate.Size = new System.Drawing.Size(70, 21);
            this.tbRate.TabIndex = 11;
            this.tbRate.TextChanged += new System.EventHandler(this.tbRate_TextChanged);
            // 
            // tbStep
            // 
            this.tbStep.Location = new System.Drawing.Point(254, 17);
            this.tbStep.Name = "tbStep";
            this.tbStep.Size = new System.Drawing.Size(70, 21);
            this.tbStep.TabIndex = 10;
            this.tbStep.TextChanged += new System.EventHandler(this.tbStep_TextChanged);
            // 
            // labelRate
            // 
            this.labelRate.AutoSize = true;
            this.labelRate.Location = new System.Drawing.Point(189, 44);
            this.labelRate.Name = "labelRate";
            this.labelRate.Size = new System.Drawing.Size(59, 12);
            this.labelRate.TabIndex = 8;
            this.labelRate.Text = "扫描速度:";
            // 
            // labelStep
            // 
            this.labelStep.AutoSize = true;
            this.labelStep.Location = new System.Drawing.Point(189, 20);
            this.labelStep.Name = "labelStep";
            this.labelStep.Size = new System.Drawing.Size(59, 12);
            this.labelStep.TabIndex = 7;
            this.labelStep.Text = "电位步进:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(148, 44);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(11, 12);
            this.label3.TabIndex = 5;
            this.label3.Text = "V";
            // 
            // labelEnd
            // 
            this.labelEnd.AutoSize = true;
            this.labelEnd.Location = new System.Drawing.Point(7, 44);
            this.labelEnd.Name = "labelEnd";
            this.labelEnd.Size = new System.Drawing.Size(59, 12);
            this.labelEnd.TabIndex = 4;
            this.labelEnd.Text = "终止电位:";
            // 
            // tbEnd
            // 
            this.tbEnd.Location = new System.Drawing.Point(72, 41);
            this.tbEnd.Name = "tbEnd";
            this.tbEnd.Size = new System.Drawing.Size(70, 21);
            this.tbEnd.TabIndex = 3;
            this.tbEnd.TextChanged += new System.EventHandler(this.tbEnd_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(146, 20);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(11, 12);
            this.label5.TabIndex = 2;
            this.label5.Text = "V";
            // 
            // tbBegin
            // 
            this.tbBegin.Location = new System.Drawing.Point(72, 17);
            this.tbBegin.Name = "tbBegin";
            this.tbBegin.Size = new System.Drawing.Size(70, 21);
            this.tbBegin.TabIndex = 1;
            this.tbBegin.TextChanged += new System.EventHandler(this.tbBegin_TextChanged);
            // 
            // labelBegin
            // 
            this.labelBegin.AutoSize = true;
            this.labelBegin.Location = new System.Drawing.Point(7, 20);
            this.labelBegin.Name = "labelBegin";
            this.labelBegin.Size = new System.Drawing.Size(59, 12);
            this.labelBegin.TabIndex = 0;
            this.labelBegin.Text = "初始电位:";
            // 
            // PSSDKBasicAsyncExample1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1172, 457);
            this.Controls.Add(this.grpParamSet);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.grpConsole);
            this.Controls.Add(this.grpMeasurement);
            this.Controls.Add(this.grpDevice);
            this.Controls.Add(this.grpConnection);
            this.Name = "PSSDKBasicAsyncExample1";
            this.Text = "线性扫描伏安法（LSV）";
            this.grpConnection.ResumeLayout(false);
            this.grpDevice.ResumeLayout(false);
            this.grpDevice.PerformLayout();
            this.grpMeasurement.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvMeasurement)).EndInit();
            this.grpConsole.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.grpParamSet.ResumeLayout(false);
            this.grpParamSet.PerformLayout();
            this.ResumeLayout(false);


        }


        // 
        // psCommSimpleWinForms  通过控件修改初始化的时候容易被改掉
        // 
        /* 
         /*
         PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.EnableBluetooth = false;
         PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.EnableSerialPort = false;
         PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.Parent = null;*//*
         PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.ReceiveStatus += new PalmSens.Comm.StatusEventHandler(this.psCommSimpleWinForms_ReceiveStatus);
         PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.MeasurementStarted += new System.EventHandler(this.psCommSimpleWinForms_MeasurementStarted);
         PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.MeasurementEnded += new System.EventHandler<System.Exception>(this.psCommSimpleWinForms_MeasurementEnded);
         PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.StateChanged += new PalmSens.Comm.CommManager.StatusChangedEventHandler(this.psCommSimpleWinForms_StateChanged);
         PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.Disconnected += new PalmSens.Core.Simplified.DisconnectedEventHandler(this.psCommSimpleWinForms_Disconnected);
         PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.SimpleCurveStartReceivingData += new PalmSens.Core.Simplified.PSCommSimple.SimpleCurveStartReceivingDataHandler(this.psCommSimpleWinForms_SimpleCurveStartReceivingData);
        
         */
        #endregion

        private float beginV = -0.5f;
        private float endV = 0.5f;
        private float stepV = 0.005f;
        private float rateV = 0.1f;
        private float equilibT = 8f;
        public static PalmSens.Core.Simplified.WinForms.PSCommSimpleWinForms psCommSimpleWinForms;
        //private PalmSens.Core.Simplified.WinForms.PSCommSimpleWinForms psCommSimpleWinForms;
        private System.Windows.Forms.Button btnMeasure;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.Button btnRefresh;
        private System.Windows.Forms.ComboBox cmbDevices;
        private System.Windows.Forms.GroupBox grpConnection;
        private System.Windows.Forms.GroupBox grpDevice;
        private System.Windows.Forms.GroupBox grpMeasurement;
        private System.Windows.Forms.DataGridView dgvMeasurement;
        private System.Windows.Forms.GroupBox grpConsole;
        private System.Windows.Forms.ListBox lbConsole;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbDeviceStatus;
        private System.Windows.Forms.Label lblCurrentRange;
        private System.Windows.Forms.Label lblCurrent;
        private System.Windows.Forms.TextBox tbCurrent;
        private System.Windows.Forms.Label lblVolt;
        private System.Windows.Forms.TextBox tbPotential;
        private System.Windows.Forms.Label lblPotential;
        private System.Windows.Forms.GroupBox groupBox1;
        private SDKPlot.WinForms.Plot plot;
        private SDKPlot.WinForms.Plot plot2;
        private System.Windows.Forms.GroupBox grpParamSet;
        private System.Windows.Forms.Label labelStep;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label labelEnd;
        private System.Windows.Forms.TextBox tbEnd;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tbBegin;
        private System.Windows.Forms.Label labelBegin;
        private System.Windows.Forms.Label labelRate;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox tbRate;
        private System.Windows.Forms.TextBox tbStep;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox tbEquilib;
        private System.Windows.Forms.Label labelEquilib;
    }
}

