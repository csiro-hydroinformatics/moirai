#include <stdexcept>

#include "moirai/reference_handle.h"


namespace moirai
{
	reference_handle_map::reference_handle_map()
	{
	}

	reference_handle_map& reference_handle_map::instance()
	{
		static reference_handle_map* ans = new reference_handle_map();
		return *ans;
	}

	std::shared_ptr<void> reference_handle_map::get(void* p, const create_handle_func& createPtr)
	{
		std::lock_guard<std::mutex> guard(map_mutex);
		size_t address = (size_t)p;
		auto k = pointers.find(address);
		if (k == pointers.end())
			pointers[address] = createPtr(p);
		return pointers[address];
	}

	bool reference_handle_map::has_handle(void* p)
	{
		size_t address = (size_t)p;
		auto k = pointers.find(address);
		return (k != pointers.end());
	}

	void reference_handle_map::release(size_t address)
	{
		std::lock_guard<std::mutex> guard(map_mutex);
		auto k = pointers.find(address);
		if (k == pointers.end())
			throw std::invalid_argument("shared pointer not found for specified memory address");
		std::shared_ptr<void> p = pointers[address];
		// at this point we have at least three shared_ptr referencing the resource.
		// * the local variable p in this function
		// * the shared_ptr<T> that the reference_handle that is currently being destroyed
		// * the shared_ptr<void> that is held in the pointers dictionary
		// if the use count is down to three, this is thus time to remove all shared_ptr.
		if (p.use_count() == 3)
		{
			pointers.erase(address);
		}
	}

	void reference_handle_map::release(void* p) { release((size_t)p); }

	opaque_ptr_provider::opaque_ptr_provider(const std::type_info& tinfo)
	{
		wrapped_type_info = typeinfo(tinfo);
	}

	const size_t opaque_ptr_provider::type_info_hash_code() { return wrapped_type_info.hash_code(); }

	opaque_ptr_provider::~opaque_ptr_provider() {}

	const char* opaque_ptr_provider::wrapped_type_name() const { return wrapped_type_info.name(); }

	typeinfo::typeinfo()
		: p_(nullptr)
	{}

	typeinfo::typeinfo(const std::type_info& t)
		: p_(&t)
	{}

	const char* typeinfo::name() const
	{
		return p_ ? p_->name() : "";
	}

	const size_t typeinfo::hash_code() const
	{
		return p_ ? p_->hash_code() : 0;
	}

	bool typeinfo::operator<(const typeinfo& that) const
	{
		return (p_ != that.p_) &&
			(!p_ || (that.p_ && static_cast<bool>(p_->before(*that.p_))));
	}

	bool typeinfo::operator==(const typeinfo& that) const
	{
		return (p_ == that.p_) ||
			(p_ && that.p_ && static_cast<bool>(*p_ == *that.p_));
	}

}