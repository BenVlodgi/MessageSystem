// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageSystemSubsystem.h"
#include "MessengerComponent.h"

void UMessageSystemSubsystem::MessengerComponentUpdated(UMessengerComponent* MessengerComponent)
{
	// TODO: make sure we update the correct one.

	//if (IsValid(MessengerComponent))
	//{
	//	UWorld* world = MessengerComponent->GetWorld();
	//	if (IsValid(world))
	//	{
	//		EWorldTypeEnum worldType = ToWorldTypeEnum(world->WorldType);
	//		FMessagesCollectionsStruct messagesCollections = MessagesCollectionsByWorld.FindOrAdd(worldType);
	//
	//		// Remove existing messages for this component from the AllMessagesByReceivingActor dictionary.
	//		FMessagesArrayStruct messagesArray = messagesCollections.AllMessagesBySender.FindOrAdd(MessengerComponent);
	//		for (FMessageStruct message : messagesArray.Array)
	//		{
	//			FMessagesArrayStruct targetMessagesArray = messagesCollections.AllMessagesByReceivingActor.FindOrAdd(message.TargetActor);
	//
	//			// Reverse loop over, because we may be removing from this list
	//			for (int i = targetMessagesArray.Array.Max(); i >= 0; i--)
	//			{
	//				FMessageStruct targetMessage = targetMessagesArray.Array[i];
	//				if (targetMessage.SendingComponent == MessengerComponent)
	//				{
	//					targetMessagesArray.Array.RemoveAt(i);
	//				}
	//			}
	//		}
	//
	//		// Remove messages from AllMessagesBySender dictionary.
	//		messagesCollections.AllMessagesBySender.Remove(MessengerComponent);
	//
	//
	//
	//		// Add messages for this component to the AllMessagesBySender & AllMessagesBySender dictionaries.
	//		messagesCollections.AllMessengerComponents.AddUnique(MessengerComponent);
	//
	//		for (FMessageStruct message : MessengerComponent->MessageEvents)
	//		{
	//			messagesArray.Array.Add(message);
	//
	//			FMessagesArrayStruct targetMessagesArray = messagesCollections.AllMessagesByReceivingActor.FindOrAdd(message.TargetActor);
	//			targetMessagesArray.Array.Add(message);
	//
	//			// Trying to commit changes made here
	//			messagesCollections.AllMessagesByReceivingActor.Add(message.TargetActor, targetMessagesArray);
	//		}
	//
	//		// Trying to commit changes made here
	//		MessagesCollectionsByWorld.Add(worldType, messagesCollections);
	//	}
	//}
	//
	//OnMessengerComponentUpdated.Broadcast(MessengerComponent);
}

void UMessageSystemSubsystem::MessengerComponentRemoved(UMessengerComponent* MessengerComponent)
{
	// TODO: Make sure we remove this component. It might be null already.

	//if (IsValid(MessengerComponent))
	//{
	//	UWorld* world = MessengerComponent->GetWorld();
	//	if (IsValid(world))
	//	{
	//		EWorldTypeEnum worldType = ToWorldTypeEnum(world->WorldType);
	//		FMessagesCollectionsStruct messagesCollections = MessagesCollectionsByWorld.FindOrAdd(worldType);
	//
	//		// Remove existing messages for this component from the AllMessagesByReceivingActor dictionary.
	//		FMessagesArrayStruct messagesArray = messagesCollections.AllMessagesBySender.FindOrAdd(MessengerComponent);
	//		for (FMessageStruct message : messagesArray.Array)
	//		{
	//			FMessagesArrayStruct targetMessagesArray = messagesCollections.AllMessagesByReceivingActor.FindOrAdd(message.TargetActor);
	//
	//			// Reverse loop over, because we may be removing from this list
	//			for (int i = targetMessagesArray.Array.Max(); i >= 0; i--)
	//			{
	//				FMessageStruct targetMessage = targetMessagesArray.Array[i];
	//				if (targetMessage.SendingComponent == MessengerComponent)
	//				{
	//					targetMessagesArray.Array.RemoveAt(i);
	//				}
	//			}
	//		}
	//
	//		// Remove messages from AllMessagesBySender dictionary.
	//		messagesCollections.AllMessagesBySender.Remove(MessengerComponent);
	//
	//		// Remove component from list of components
	//		messagesCollections.AllMessengerComponents.Remove(MessengerComponent);
	//	}
	//}
	//
	//OnMessengerComponentRemoved.Broadcast(MessengerComponent);
}

void UMessageSystemSubsystem::AddMessage(FMessageStruct Message, bool BroadcastUpdate)
{
	if (Message.SendingComponent.IsValid())
	{
		UMessengerComponent* messengerComponent = Message.SendingComponent.Get();
		if (IsValid(messengerComponent)) // This check may not be nessesary
		{
			UWorld* world = messengerComponent->GetWorld();
			if (IsValid(world))
			{
				EWorldTypeEnum worldType = ToWorldTypeEnum(world->WorldType);
				FMessagesCollectionsStruct messagesCollections = MessagesCollectionsByWorld.FindOrAdd(worldType);

				messagesCollections.AllMessengerComponents.Add(messengerComponent);
				messagesCollections.AllMessages.Add(Message.ID, Message);

				FGuidArrayStruct* messagesBySender = messagesCollections.AllMessagesBySender.Find(messengerComponent);
				if (messagesBySender)
				{
					messagesBySender->Array.Add(Message.ID);
				}

				FGuidArrayStruct* messagesByReceivingActor = messagesCollections.AllMessagesByReceivingActor.Find(Message.TargetActor);
				if (messagesByReceivingActor)
				{
					messagesByReceivingActor->Array.Add(Message.ID);
				}

				// Ensure to commit changes made here
				MessagesCollectionsByWorld.Add(worldType, messagesCollections);

				if (BroadcastUpdate)
				{
					OnMessengerComponentUpdated.Broadcast(messengerComponent);
				}
			}
		}
	}
}

void UMessageSystemSubsystem::RemoveMessage(FMessageStruct Message, bool BroadcastUpdate)
{
	if (Message.SendingComponent.IsValid())
	{
		UMessengerComponent* messengerComponent = Message.SendingComponent.Get();
		if (IsValid(messengerComponent)) // This check may not be nessesary
		{
			UWorld* world = messengerComponent->GetWorld();
			if (IsValid(world))
			{
				EWorldTypeEnum worldType = ToWorldTypeEnum(world->WorldType);
				FMessagesCollectionsStruct* messagesCollections = MessagesCollectionsByWorld.Find(worldType);

				if (messagesCollections)
				{
					//messagesCollections->AllMessengerComponents.Remove(messengerComponent); // We still want to know about the component even if it doesn't have messages
					messagesCollections->AllMessages.Remove(Message.ID);

					FGuidArrayStruct* messagesBySender = messagesCollections->AllMessagesBySender.Find(messengerComponent);
					if (messagesBySender)
					{
						messagesBySender->Array.Remove(Message.ID);
					}

					FGuidArrayStruct* messagesByReceivingActor = messagesCollections->AllMessagesByReceivingActor.Find(Message.TargetActor);
					if (messagesByReceivingActor)
					{
						messagesByReceivingActor->Array.Remove(Message.ID);
					}
				}

				if (BroadcastUpdate)
				{
					OnMessengerComponentUpdated.Broadcast(messengerComponent);
				}
			}
		}
	}
}

void UMessageSystemSubsystem::UpdateMessage(FMessageStruct Message, bool BroadcastUpdate)
{
	if (Message.SendingComponent.IsValid())
	{
		UMessengerComponent* messengerComponent = Message.SendingComponent.Get();
		if (IsValid(messengerComponent)) // This check may not be nessesary
		{
			RemoveMessage(Message, false);
			AddMessage(Message, false);

			if (BroadcastUpdate)
			{
				OnMessengerComponentUpdated.Broadcast(messengerComponent);
			}
		}
	}
}
