#define CONTROLLER_AXIS_RANGE_MIN 32767
#define CONTROLLER_AXIS_RANGE_MAX -32768
#define AXIS_RANGE 32768

/// Buttons available from a game controller
enum ControllerButton {
    eControllerButtonInvalid = -1,
    eControllerButtonA,
    eControllerButtonB,
    eControllerButtonX,
    eControllerButtonY,
    eControllerButtonBack,
    eControllerButtonGuide,
    eControllerButtonStart,
    eControllerButtonLeftStick,
    eControllerButtonRightStick,
    eControllerButtonLeftShoulder,
    eControllerButtonRightShoulder,
    eControllerButtonDPadUp,
    eControllerButtonDPadDown,
    eControllerButtonDPadLeft,
    eControllerButtonDPadRight,
    /// Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button
    eControllerButtonMisc1,
    /// Xbox Elite paddle P1 (upper left, facing the back)
    eControllerButtonPaddle1,
    /// Xbox Elite paddle P3 (upper right, facing the back)
    eControllerButtonPaddle2,
    /// Xbox Elite paddle P2 (lower left, facing the back)
    eControllerButtonPaddle3,
    /// Xbox Elite paddle P4 (lower right, facing the back)
    eControllerButtonPaddle4,
    /// PS4/PS5 touchpad button
    eControllerButtonPaddleTouchpad,
    eControllerButtonMax
};

/// Axes available from a game controller
enum ControllerAxis {
    eControllerAxisInvalid = -1;
    eControllerAxisLeftX,
    eControllerAxisLeftY,
    eControllerAxisRightX,
    eControllerAxisRightY,
    eControllerAxisTriggerLeft,
    eControllerAxisTriggerRight,
    eControllerAxisMax
};

enum ControllerPOV {
  ePOVCenter = 0,
  ePOVUp = 1,
  ePOVRight = 2,
  ePOVUpRight = 3,
  ePOVDown = 4,
  ePOVDownRight = 6,
  ePOVLeft = 8,
  ePOVUpLeft = 9,
  ePOVDownLeft = 12
};

/// Does a single mouse click. 
import void ClickMouse(int button);

/// Returns the number of gamecontrollers found
import int ControllerCount(); 

managed struct Controller {
  readonly int ID;
  readonly int ButtonCount;
  readonly int AxesCount;
  readonly ControllerPOV POV;

  /// Opens specified controller. (0-15)
  import static Controller* Open (int ID); // $AUTOCOMPLETESTATICONLY$

  /// Closes controller
  import void Close ();

  /// Returns if the controller is currently plugged or not (true / false) 
  import bool Plugged ();

  /// Returns the controller name
  import String GetName ();

  /// Returns axis value bynumber. (0-5)
  import int GetAxis (int axis);

  /// Returns POV value. (0-8)
  import int GetPOV();

  /// Rumbles the Controller for Duration (in loops). Left and right are motors. Values go from 0 to 65535
  import void Rumble(int left,int right,int duration);

  /// Returns true when the specified button is currently down. (0-31)
  import bool IsButtonDown (int button);

  /// Returns the first button the player hits on the controller, otherwise returns -1. (0-31)
  import int PressAnyKey();

  /// Returns the status of the controller battery. (-1 - 5) UNKNOWN = -1, LESS THAN 5% = 0, LESS THAN 20% = 1, LESS THAN 70% = 2, 100% = 3, WIRED = 4, MAX = 5 
  import int BatteryStatus();

  /// Returns true when the specified button is currently down (single press). (0-31)
  import bool IsButtonDownOnce(int button);
};