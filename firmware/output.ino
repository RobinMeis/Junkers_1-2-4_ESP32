void setVoltage(int voltage) {
  if (voltage > OPV_U_MAX) voltage = OPV_U_MAX;
  int dac = ((DAC_MAX/DAC_U_MAX)*(voltage*(DAC_U_MAX/OPV_U_MAX)));
  dacWrite(26, dac);
}
