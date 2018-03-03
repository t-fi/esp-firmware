class LedDriverRev1 {
private:
  uint8_t col[64];
  uint8_t colRev[2];

  uint8_t fadingCol[256];
  bool fadeStrip[4];

  uint8_t regLow;
  uint8_t colorLow;
  uint8_t colorHigh;
  uint8_t dataI;
  uint8_t dataIp1;
  uint8_t dataColH;

  void initPCA9685();
  void initCol();
  uint16_t calcFadeSteps(uint8_t stripNbr);
  void fade();

public:
  LedDriverRev1 ();
  void parseColors(uint8_t *data, uint8_t length);
  void setFade(uint8_t stripNbr, uint16_t *data);
  void setFadeSpeed(uint8_t stripNbr, uint8_t speed);
  void startFade(bool *fadeStrip);

  virtual ~LedDriverRev1 (){}
};
