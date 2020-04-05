/*! \file
 * \copyright
 * COPYRIGHT NOTICE: (c) 2017-2018 Neonode Technologies AB. All rights reserved.
 *
 * \sa file://DumpMessage.h
 */

// Header Files
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <zForceCommon.h>
#include <Message.h>
#include "zforce_proxy/zforce_error_string.h"
#include "zforce_proxy/zforce_dump_message.h"

// Ardonyx internal
#include "zforce_proxy/zforce_proxy_common.h"

static void DumpEnableMessage (Message * message);
static void DumpDisableMessage (Message * message);
static void DumpOperationModesMessage (Message * message);
static void DumpResolutionMessage (Message * message);
static void DumpTouchActiveArea (Message * message);
static void DumpReverseTouchActiveArea (Message * message);
static void DumpMcuUniqueIdentifier (Message * message);
static void DumpTouchMessage (Message * message);
static void DumpNumberOfTrackedObjectsMessage (Message * message);
static void DumpFingerFrequencyMessage (Message * message);
static void DumpIdleFrequencyMessage (Message * message);
static void DumpDetectedObjectSizeRestrictionMessage (Message * message);
static void DumpOffsetMessage (Message * message);
static void DumpHidDisplaySizeMessage (Message * message);
static void DumpFlipXYMessage (Message * message);
static void DumpReflectiveEdgeFilterMessage (Message * message);
static void DumpMergeTouchesMessage (Message * message);
static void DumpMessageError (Message * message);

// Exported Functions

/*!
 * \sa DumpMessage.h
 */
void DumpMessage (Message * message)
{
    LOG_INFO ("Message Serial Number = %" PRIu64 "\n", message->SerialNumber);

    switch (message->MessageType)
    {
    case EnableMessageType:
        DumpEnableMessage (message);
        break;

    case DisableMessageType:
        DumpDisableMessage (message);
        break;

    case OperationModesMessageType:
        DumpOperationModesMessage (message);
        break;

    case ResolutionMessageType:
        DumpResolutionMessage (message);
        break;

    case ReverseTouchActiveAreaMessageType:
        DumpReverseTouchActiveArea (message);
        break;

    case TouchActiveAreaMessageType:
        DumpTouchActiveArea (message);
        break;

    case TouchMessageType:
        DumpTouchMessage (message);
        break;

    case McuUniqueIdentifierMessageType:
        DumpMcuUniqueIdentifier(message);
        break;

    case NumberOfTrackedObjectsMessageType:
        DumpNumberOfTrackedObjectsMessage (message);
        break;

    case FingerFrequencyMessageType:
        DumpFingerFrequencyMessage (message);
        break;

    case IdleFrequencyMessageType:
        DumpIdleFrequencyMessage (message);
        break;

    case DetectedObjectSizeRestrictionMessageType:
        DumpDetectedObjectSizeRestrictionMessage (message);
        break;

    case OffsetMessageType:
        DumpOffsetMessage (message);
        break;

    case HidDisplaySizeMessageType:
        DumpHidDisplaySizeMessage (message);
        break;

    case FlipXYMessageType:
        DumpFlipXYMessage (message);
        break;

    case ReflectiveEdgeFilterMessageType:
        DumpReflectiveEdgeFilterMessage(message);
        break;

    case MergeTouchesMessageType:
        DumpMergeTouchesMessage(message);
        break;

    default:
        LOG_INFO ("Unknown Message received!\n");
        break;
    }

}

// Local (static) Functions.
static void DumpEnableMessage (Message * message)
{
    EnableMessage * enableMessage = (EnableMessage *)message;

    LOG_INFO ("Enable Message received.\n");

    if (message->Error)
    {
        DumpMessageError (message);
    }
    else
    {
        LOG_INFO ("  Enabled: %s.\n", enableMessage->Enabled ? "Yes" : "No");
        LOG_INFO ("  Continuous: %s.\n", enableMessage->ContinuousMode ? "Yes" : "No");
        LOG_INFO ("  NumberOfMessages: %d.\n", enableMessage->NumberOfMessages);
    }

}

static void DumpMcuUniqueIdentifier(Message * message)
{
    McuUniqueIdentifierMessage * mcuUniqueIdentifierMessage = (McuUniqueIdentifierMessage *)message;

    LOG_INFO ("Mcu Unique Identifier Message received.\n");

    if (message->Error)
    {
        DumpMessageError (message);
    }
    else
    {
        LOG_INFO("Mcu Unique Identifier is: ");
        for (uint32_t i = 0; i < mcuUniqueIdentifierMessage->BufferSize; i++)
        {
            LOG_INFO("%02X", mcuUniqueIdentifierMessage->McuUniqueIdentifier[i]);
        }
        LOG_INFO("\n");
    }
}

static void DumpDisableMessage (Message * message)
{
    DisableMessage * disableMessage = (DisableMessage *)message;

    LOG_INFO ("Disable Message received.\n");

    if (message->Error)
    {
        DumpMessageError (message);
    }
    else
    {
        LOG_INFO ("  Disabled: %s.\n", disableMessage->Disabled ? "Yes" : "No");
    }

}

static void DumpOperationModesMessage (Message * message)
{
    OperationModesMessage * operationModesMessage =
        (OperationModesMessage *) message;
    OperationModes          mask;
    OperationModes          values;

    LOG_INFO ("Operation Modes Message received.\n");

    if (message->Error)
    {
        DumpMessageError (message);
    }
    else
    {
        mask = operationModesMessage->Mask;
        values = operationModesMessage->Values;

        LOG_INFO ("Modes reported:\n");

        if (NoOperationMode == mask)
        {
            LOG_INFO ("   No mode set.\n");
        }

        if (DetectionMode & mask)
        {
            LOG_INFO ("   Detection mode %s.\n",
                    (DetectionMode & values) ? "on" : "off");
        }

        if (SignalsMode & mask)
        {
            LOG_INFO ("   Signals mode %s.\n",
                    (SignalsMode & values) ? "on" : "off");
        }

        if (LedLevelsMode & mask)
        {
            LOG_INFO ("   LED levels mode %s.\n",
                    (LedLevelsMode & values) ? "on" : "off");
        }

        if (DetectionHidMode & mask)
        {
            LOG_INFO ("   Detection HID mode %s.\n",
                    (DetectionHidMode & values) ? "on" : "off");
        }

        if (GesturesMode & mask)
        {
            LOG_INFO ("   Gestures mode %s.\n",
                    (GesturesMode & values) ? "on" : "off");
        }

        LOG_INFO ("End of report.\n");
    }

}

static void DumpResolutionMessage (Message * message)
{
    ResolutionMessage * resolutionMessage = (ResolutionMessage *)message;
    LOG_INFO ("Resolution Message received.\n");

    if (message->Error)
    {
        DumpMessageError (message);
    }

    if (resolutionMessage->HasX || resolutionMessage->HasY ||
        resolutionMessage->HasZ)
    {
        if (resolutionMessage->HasX)
        {
            LOG_INFO ("  X: %8d", resolutionMessage->X);
        }

        if (resolutionMessage->HasX && resolutionMessage->HasY)
        {
            LOG_INFO (", ");
        }

        if (resolutionMessage->HasY)
        {
            LOG_INFO ("Y: %8d", resolutionMessage->Y);
        }

        if (resolutionMessage->HasZ &&
            (resolutionMessage->HasX || resolutionMessage->HasY))
        {
            LOG_INFO (", ");
        }

        if (resolutionMessage->HasZ)
        {
            LOG_INFO ("Z: %8d", resolutionMessage->Z);
        }

    }

    LOG_INFO (".\n");
}

static void DumpReflectiveEdgeFilterMessage(Message * message)
{
    ReflectiveEdgeFilterMessage * reflectiveEdgeFilterMessage = (ReflectiveEdgeFilterMessage *)message;
    LOG_INFO("ReflectiveEdgeFilter Message received.\n");

    if (message->Error)
    {
        DumpMessageError(message);
    }

    if (NULL != reflectiveEdgeFilterMessage)
    {
        LOG_INFO("The reflective edge filter is %s.\n", reflectiveEdgeFilterMessage->FilterIsOn ? "on" : "off");
    }

}

static void DumpMergeTouchesMessage(Message * message)
{
    MergeTouchesMessage * mergeTouchesMessage = (MergeTouchesMessage *)message;
    LOG_INFO("MergeTouches Message received.\n");

    if (message->Error)
    {
        DumpMessageError(message);
    }

    if (NULL != mergeTouchesMessage)
    {
        LOG_INFO("The touches are %s.\n", mergeTouchesMessage->AreTouchesMerged ? "merged" : "not merged");
    }

}

static void DumpReverseTouchActiveArea (Message * message)
{
    ReverseTouchActiveAreaMessage * reverseTouchActiveAreaMessage;

    LOG_INFO ("Reverse Touch Active Area Message received.\n");

    if (message->Error)
    {
        DumpMessageError (message);
    }
    else
    {
        reverseTouchActiveAreaMessage = (ReverseTouchActiveAreaMessage *) message;

        LOG_INFO ("  X reversed is %s\n",
                reverseTouchActiveAreaMessage->XIsReversed ? "true" : "false");

        LOG_INFO ("  Y reversed is %s\n",
                reverseTouchActiveAreaMessage->YIsReversed ? "true" : "false");
    }
}

static void DumpFlipXYMessage (Message * message)
{
    FlipXYMessage * flipXYMessage;

    LOG_INFO ("Flip XY Message received.\n");

    if (message->Error)
    {
        DumpMessageError (message);
    }
    else
    {
        flipXYMessage = (FlipXYMessage *) message;

        LOG_INFO ("  FlipXY is %s\n",
                flipXYMessage->AxesAreFlipped ? "true" : "false");
    }

}

static void DumpTouchActiveArea (Message * message)
{
    TouchActiveAreaMessage * touchActiveAreaMessage;

    LOG_INFO ("Touch Active Area Message received.\n");

    if (message->Error)
    {
        DumpMessageError (message);
    }
    else
    {
        touchActiveAreaMessage = (TouchActiveAreaMessage *) message;

        if (touchActiveAreaMessage->HasX)
        {
            LOG_INFO ("   X lower boundary = %u",
                    touchActiveAreaMessage->LowerBoundaryX);
            LOG_INFO ("   X upper boundary = %u\n",
                    touchActiveAreaMessage->UpperBoundaryX);
        }

        if (touchActiveAreaMessage->HasY)
        {
            LOG_INFO ("   Y lower boundary = %u",
                    touchActiveAreaMessage->LowerBoundaryY);
            LOG_INFO ("   Y upper boundary = %u\n",
                    touchActiveAreaMessage->UpperBoundaryY);
        }

    }

}

static void DumpTouchMessage (Message * message)
{
    TouchMessage * touchMessage = (TouchMessage *)message;
    char         * eventString = NULL;

    if (message->Error)
    {
        DumpMessageError (message);
    }

    switch (touchMessage->Event)
    {
    case DownEvent:
        eventString = "Down";
        break;

    case MoveEvent:
        eventString = "Move";
        break;

    case UpEvent:
        eventString = "Up";
        break;

    case InvalidEvent:
        eventString = "Invalid";
        break;

    case GhostEvent:
        eventString = "Ghost";
        break;

    default:
        eventString = "Unknown";
        break;
    }

    LOG_INFO ("Id: %2d, Event: %7s", touchMessage->Id,
            eventString);

    if (touchMessage->HasX || touchMessage->HasY ||
        touchMessage->HasZ)
    {
        LOG_INFO (", ");

        if (touchMessage->HasX)
        {
            LOG_INFO ("X: %8d", touchMessage->X);
        }

        if (touchMessage->HasX && touchMessage->HasY)
        {
            LOG_INFO (", ");
        }

        if (touchMessage->HasY)
        {
            LOG_INFO ("Y: %8d", touchMessage->Y);
        }

        if (touchMessage->HasZ &&
            (touchMessage->HasX || touchMessage->HasY))
        {
            LOG_INFO (", ");
        }

        if (touchMessage->HasZ)
        {
            LOG_INFO ("Z: %8d", touchMessage->Z);
        }
    }

    if (touchMessage->HasSizeX || touchMessage->HasSizeY ||
        touchMessage->HasSizeZ)
    {
        LOG_INFO (", ");
        if (touchMessage->HasSizeX)
        {
            LOG_INFO ("X Size: %8d", touchMessage->SizeX);
        }

        if (touchMessage->HasSizeX && touchMessage->HasSizeY)
        {
            LOG_INFO (", ");
        }

        if (touchMessage->HasSizeY)
        {
            LOG_INFO ("Y Size: %8d", touchMessage->SizeY);
        }

        if (touchMessage->HasSizeZ &&
            (touchMessage->HasSizeX || touchMessage->HasSizeY))
        {
            LOG_INFO (", ");
        }

        if (touchMessage->HasSizeZ)
        {
            LOG_INFO ("Z Size: %8d", touchMessage->SizeZ);
        }

    }

    LOG_INFO (".\n");
}

static void DumpNumberOfTrackedObjectsMessage (Message * message)
{
    NumberOfTrackedObjectsMessage * numberOfTrackedObjectsMessage
        = (NumberOfTrackedObjectsMessage *)message;

    LOG_INFO ("Number Of Tracked Objects Message received.\n");

    if (message->Error)
    {
        DumpMessageError (message);
    }
    else
    {
        LOG_INFO ("  Number: %u.\n",
                numberOfTrackedObjectsMessage->NumberOfTrackedObjects);
    }

}

static void DumpFingerFrequencyMessage (Message * message)
{
    FingerFrequencyMessage * fingerFrequencyMessage =
        (FingerFrequencyMessage *)message;

    LOG_INFO ("Finger Frequency Message received.\n");

    if (message->Error)
    {
        DumpMessageError (message);
    }
    else
    {
        LOG_INFO ("  Frequency: %d.\n", fingerFrequencyMessage->Frequency);
    }

}

static void DumpIdleFrequencyMessage (Message * message)
{
    IdleFrequencyMessage * idleFrequencyMessage =
        (IdleFrequencyMessage *)message;

    LOG_INFO ("Idle Frequency Message received.\n");

    if (message->Error)
    {
        DumpMessageError (message);
    }
    else
    {
        LOG_INFO ("  Frequency: %d.\n", idleFrequencyMessage->Frequency);
    }

}

static void DumpDetectedObjectSizeRestrictionMessage (Message * message)
{
    DetectedObjectSizeRestrictionMessage * sizeMessage =
        (DetectedObjectSizeRestrictionMessage *) message;

    LOG_INFO ("Detected Object Size Restriction Message Received \n");

    LOG_INFO ("MinimumSize Enabled : %s \n",
            (sizeMessage->HasMinimumSize == true) ? "True" : "False");

    if (sizeMessage->HasMinimumSize)
    {
        LOG_INFO ("MinimumSize : %u \n", sizeMessage->MinimumSize);
    }

    LOG_INFO ("MaximumSize Enabled : %s \n",
            (sizeMessage->HasMaximumSize == true) ? "True" : "False");

    if (sizeMessage->HasMaximumSize)
    {
        LOG_INFO ("MaximumSize : %u \n", sizeMessage->MaximumSize);
    }

}

static void DumpOffsetMessage (Message * message)
{
    OffsetMessage * offsetMessage = (OffsetMessage *)message;

    LOG_INFO ("Offset Message received.\n");

    if (message->Error)
    {
        DumpMessageError (message);
    }
    else
    {
        if (true == offsetMessage->HasOffsetX)
        {
            LOG_INFO ("  OffsetX: %d.\n", offsetMessage->OffsetX);
        }

        if (true == offsetMessage->HasOffsetY)
        {
            LOG_INFO ("  OffsetY: %d.\n", offsetMessage->OffsetY);
        }
    }

}

static void DumpHidDisplaySizeMessage (Message * message)
{
    HidDisplaySizeMessage * hidDisplaySizeMessage = (HidDisplaySizeMessage *)message;

    LOG_INFO ("HidDisplaySize Message received.\n");

    if (message->Error)
    {
        DumpMessageError (message);
    }
    else
    {

        if (true == hidDisplaySizeMessage->HasSizeX)
        {
            LOG_INFO ("  SizeX: %d.\n", hidDisplaySizeMessage->SizeX);
        }

        if (true == hidDisplaySizeMessage->HasSizeY)
        {
            LOG_INFO ("  SizeY: %d.\n", hidDisplaySizeMessage->SizeY);
        }

    }

}

static void DumpMessageError (Message * message)
{
    LOG_ERROR ("**  Error: (%d) %s\n",
            message->ErrorCode,
            ErrorString (message->ErrorCode));
}
