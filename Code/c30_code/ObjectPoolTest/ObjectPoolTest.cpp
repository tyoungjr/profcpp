#include "ObjectPoolTest.h"
import <memory>;
import <vector>;
import <set>;
import <algorithm>;

import object_pool;
import serial;

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void ObjectPoolTest::setUp() { }
void ObjectPoolTest::tearDown() { }

void ObjectPoolTest::testSimple()
{
	Assert::IsTrue(0 < 1);
}

void ObjectPoolTest::testException()
{
	Assert::ExpectException<std::invalid_argument>(
		[] { throw std::invalid_argument{ "Error" }; },
		L"Unknown exception caught.");
}

void ObjectPoolTest::testCreation()
{
	// IF nothing

	// WHEN creating an ObjectPool
	ObjectPool<Serial> myPool;

	// THEN no exception is thrown
}

void ObjectPoolTest::testAcquire()
{
	// IF an ObjectPool has been created for Serial objects
	ObjectPool<Serial> myPool;
	// WHEN acquiring an object
	auto serial{ myPool.acquireObject() };
	// THEN we get a valid Serial object
	Assert::IsTrue(serial->getSerialNumber() >= 0);
}

void ObjectPoolTest::testExclusivity()
{
	// IF an ObjectPool has been created for Serial objects
	ObjectPool<Serial> myPool;
	// WHEN acquiring several objects from the pool
	const size_t numberOfObjectsToRetrieve{ 10 };
	vector<shared_ptr<Serial>> retrievedSerials;
	set<size_t> seenSerialNumbers;

	for (size_t i{ 0 }; i < numberOfObjectsToRetrieve; i++) {
		auto nextSerial{ myPool.acquireObject() };

		// Add the retrieved Serial to the vector to keep it 'alive',
		// and add the serial number to the set.
		retrievedSerials.push_back(nextSerial);
		seenSerialNumbers.insert(nextSerial->getSerialNumber());
	}

	// THEN all retrieved serial numbers are different.
	Assert::AreEqual(numberOfObjectsToRetrieve, seenSerialNumbers.size());
}

void ObjectPoolTest::testRelease()
{
	// IF an ObjectPool has been created for Serial objects
	ObjectPool<Serial> myPool;
	// AND we acquired and released 10 objects from the pool, while
	//     remembering their raw pointers
	const size_t numberOfObjectsToRetrieve{ 10 };
	// A vector to remember all raw pointers that have been handed out by the pool.
	vector<Serial*> retrievedSerialPointers;
	vector<shared_ptr<Serial>> retrievedSerials;
	for (size_t i{ 0 }; i < numberOfObjectsToRetrieve; i++) {
		auto object{ myPool.acquireObject() };
		retrievedSerialPointers.push_back(object.get());
		// Add the retrieved Serial to the vector to keep it 'alive'.
		retrievedSerials.push_back(object);
	}
	// Release all objects back to the pool.
	retrievedSerials.clear();

	// The above loop has created 10 Serial objects, with 10 different
	// addresses, and released all 10 Serial objects back to the pool.

	// WHEN again retrieving 10 objects from the pool, and
	//      remembering their raw pointers.
	vector<Serial*> newlyRetrievedSerialPointers;
	for (size_t i{ 0 }; i < numberOfObjectsToRetrieve; i++) {
		auto object{ myPool.acquireObject() };
		newlyRetrievedSerialPointers.push_back(object.get());
		// Add the retrieved Serial to the vector to keep it 'alive'.
		retrievedSerials.push_back(object);
	}
	// Release all objects back to the pool.
	retrievedSerials.clear();

	// THEN all addresses of the 10 newly acquired objects must have been
	//      seen already during the first loop of acquiring 10 objects.
	//      This makes sure objects are properly re-used by the pool.
	sort(begin(retrievedSerialPointers), end(retrievedSerialPointers));
	sort(begin(newlyRetrievedSerialPointers), end(newlyRetrievedSerialPointers));
	Assert::IsTrue(retrievedSerialPointers == newlyRetrievedSerialPointers);
}
