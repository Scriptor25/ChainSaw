#include <codegen/Context.h>

#include <iostream>

std::ostream& csaw::codegen::operator<<(std::ostream& out, const Context& context)
{
	auto& types = context.Types();
	auto& functions = context.Functions();

	out << "Types:" << std::endl;
	for (auto& entry : types)
		out << '\t' << entry.first << std::endl;

	out << std::endl << "Functions: " << std::endl;
	for (auto& a : functions)
	{
		if (a.second.empty())
			continue;

		auto& callee = a.first;
		std::cout << '\t' << callee->GetName() << ':' << std::endl;
		for (auto& b : a.second)
		{
			if (b.second.empty())
				continue;

			auto& name = b.first;
			std::cout << "\t\t" << name << ':' << std::endl;
			for (auto& c : b.second)
			{
				if (!c.second)
					continue;

				auto& type = c.first;
				std::cout << "\t\t\t" << type->GetName() << std::endl;
			}
		}
	}

	return out;
}
