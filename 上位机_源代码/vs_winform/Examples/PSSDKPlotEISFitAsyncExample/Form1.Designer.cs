namespace PSSDKPlotEISFitAsyncExample
{
    partial class ZoomInWinForm
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
        private void InitializeComponent()
        {
            this.plotNyquistZoom = new SDKPlot.WinForms.Plot();
            this.plotBodeZoom = new SDKPlot.WinForms.Plot();
            this.SuspendLayout();
            // 
            // plotNyquistZoom
            // 
            this.plotNyquistZoom.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.plotNyquistZoom.AutoSize = true;
            this.plotNyquistZoom.BackColor = System.Drawing.Color.White;
            this.plotNyquistZoom.Location = new System.Drawing.Point(3, 2);
            this.plotNyquistZoom.MarkerSize = 5;
            this.plotNyquistZoom.MarkerType = OxyPlot.MarkerType.Circle;
            this.plotNyquistZoom.Name = "plotNyquistZoom";
            this.plotNyquistZoom.Size = new System.Drawing.Size(1005, 532);
            this.plotNyquistZoom.TabIndex = 4;
            this.plotNyquistZoom.Title = "Nyquist";
            this.plotNyquistZoom.XAxisLabel = null;
            this.plotNyquistZoom.XAxisType = SDKPlot.AxisType.Linear;
            this.plotNyquistZoom.YAxisLabel = null;
            this.plotNyquistZoom.YAxisSecondaryLabel = null;
            this.plotNyquistZoom.YAxisSecondaryType = SDKPlot.AxisType.Linear;
            this.plotNyquistZoom.YAxisType = SDKPlot.AxisType.Linear;
            // 
            // plotBodeZoom
            // 
            this.plotBodeZoom.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.plotBodeZoom.AutoSize = true;
            this.plotBodeZoom.BackColor = System.Drawing.Color.White;
            this.plotBodeZoom.Location = new System.Drawing.Point(1014, 2);
            this.plotBodeZoom.MarkerSize = 5;
            this.plotBodeZoom.MarkerType = OxyPlot.MarkerType.Circle;
            this.plotBodeZoom.Name = "plotBodeZoom";
            this.plotBodeZoom.Size = new System.Drawing.Size(211, 532);
            this.plotBodeZoom.TabIndex = 4;
            this.plotBodeZoom.Title = "Bode";
            this.plotBodeZoom.XAxisLabel = null;
            this.plotBodeZoom.XAxisType = SDKPlot.AxisType.Linear;
            this.plotBodeZoom.YAxisLabel = null;
            this.plotBodeZoom.YAxisSecondaryLabel = null;
            this.plotBodeZoom.YAxisSecondaryType = SDKPlot.AxisType.Linear;
            this.plotBodeZoom.YAxisType = SDKPlot.AxisType.Linear;
            // 
            // ZoomInWinForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1226, 537);
            this.Controls.Add(this.plotNyquistZoom);
            this.Controls.Add(this.plotBodeZoom);
            this.Name = "ZoomInWinForm";
            this.Text = "ZoomIn放大查看";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
       /* private SDKPlot.WinForms.Plot plotNyquistZoom;
        private SDKPlot.WinForms.Plot plotBodeZoom;*/
        public SDKPlot.WinForms.Plot plotNyquistZoom;
        public SDKPlot.WinForms.Plot plotBodeZoom;
    }
}