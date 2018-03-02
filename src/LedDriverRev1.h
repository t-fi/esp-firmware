class LedDriverRev1 {
private:
  uint8_t col[64];
  uint8_t colRev[2];

  void initPCA9685();
  void initCol();

public:
  LedDriverRev1 ();
  void parseColors(uint8_t *data, int length);
  virtual ~LedDriverRev1 (){}
};
